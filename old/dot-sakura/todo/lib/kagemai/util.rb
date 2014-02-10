=begin
  util.rb - utilities
  
  Copyright(C) 2002-2005 FUKUOKA Tomoyuki.
  
  This file is part of KAGEMAI.  
  
  KAGEMAI is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  
  $Id: util.rb,v 1.7.2.4 2005/01/20 11:32:51 fukuoka Exp $
=end

require 'erb'
require 'cgi'
require 'parsedate'
require 'thread'
require 'kagemai/error'
require 'kagemai/logger'

class Array
  unless method_defined?(:sort_by) then
    def sort_by()
      self.collect {|i| [yield(i), i] }.
        sort {|a,b| a[0] <=> b[0] }.
        collect! {|i| i[1]}
    end
  end
end

class String
  def quote(prefix = '> ')
    return prefix + self.gsub(/\n/m, "\n#{prefix}");
  end

  def escape_h()
    CGI::escapeHTML(self)
  end

  def unescape_h()
    CGI::unescapeHTML(self)
  end

  def escape_u()
    CGI::escape(self)
  end

  def unescape_u()
    CGI::unescape(self)
  end
end

class Time
  @@week_of_days = nil
  
  def self.parsedate(date, cyear = false)
    # from Ruby FAQ.
    ary = ParseDate.parsedate(date, cyear)
    if ary.size >= 6 && ary[6] == 'GMT' then
      Time::gm(*ary[0..-3])
    else
      Time::local(*ary[0..-3])
    end
  end
  
  def format()
    # 将来的には、lang に応じてフォーマットをかえたい
    self.strftime("%Y/%m/%d %H:%M:%S")
  end
  
  def format_date(lang = 'ja')
    self.strftime("%Y/%m/%d (#{Time.week_of_day(self.wday)})")
  end  
  
  def self.week_of_day(wod)
    @@week_of_days ||= [
      Kagemai::MessageBundle[:wod_sun], Kagemai::MessageBundle[:wod_mon], 
      Kagemai::MessageBundle[:wod_tue], Kagemai::MessageBundle[:wod_wed], 
      Kagemai::MessageBundle[:wod_thu], Kagemai::MessageBundle[:wod_fri], 
      Kagemai::MessageBundle[:wod_sat], 
    ]
    @@week_of_days[wod]
  end
end

def File.create(path)
  File.open(path, 'wb') {|file| }
end

def File.chmod2(mode, *filenames)
  begin
    chmod_files = filenames.find_all{|name| (File.stat(name).mode & 07777) != (mode & 07777)}
    File.chmod(mode, *chmod_files) if chmod_files.size > 0
  rescue SystemCallError => e
    prefix = 'kagemai: '
    bt = e.backtrace.join("\n" + prefix + '  ')
    STDERR.puts "#{prefix} #{e} (#{e.class})"
    STDERR.puts "#{prefix} #{bt}"
  end
end

def Dir.delete_dir(dir)
  Dir.foreach(dir) do |path|
    next if path == '.' || path == '..'
    path = dir + '/' + path
    path.untaint
    if FileTest.directory?(path)
      Dir.delete_dir(path)
    else
      File.unlink(path)
    end
  end
  Dir.rmdir(dir)
end

class ERB
  alias set_eoutvar_orig set_eoutvar

  def set_eoutvar(compiler, eoutvar = '_erbout')
    compiler.put_cmd = "#{eoutvar} << "

    cmd = []
    cmd.push "#{eoutvar} = []"
    
    compiler.pre_cmd = cmd

    cmd = []
    cmd.push("#{eoutvar}.join")

    compiler.post_cmd = cmd
  end
end

module Kagemai
  module Util
    def Util.safe(level = 4)
      result = nil
      Thread.start {
        $SAFE = level
        result = yield
      }.join
      result
    end
    
    def Util.erb_eval_file(filename, binding = TOPLEVEL_BINDING)
      src = open(filename, 'rb') {|file| ERB.new(file.read.gsub(/\r\n/, "\n")).src}
      eval(src.untaint, binding, filename)
    end
    
    def Util.eval_template(filename, template_dir, lang, b = TOPLEVEL_BINDING)
      Logger.debug('Util', "eval_template: filename = #{filename.inspect}")
      dirs = ["#{Config[:resource_dir]}/#{lang}/template/#{Config[:default_template_dir]}"]
      dirs.unshift(template_dir) if template_dir
      dirs.each do |dir|
        Logger.debug('Util', "eval_template: look up in directory: #{dir.inspect}")
        path = dir + '/' + filename
        if FileTest.exist?(path)
          return erb_eval_file(path, b)
        end
      end
      raise NoSuchTempalteError, "template file not found: '#{filename}'"
    end
    
    def Util.untaint_path(path)
      if path.include?('../')
        raise SecurityError, "Insecure path - '#{path}'"
      end
      path.untaint
    end

    def Util.untaint_digit_id(id)
      raise ParameterError, "Invalid ID - #{id.inspect}" unless id =~ /\A\d+\Z/
      id.untaint
    end
  end
end
