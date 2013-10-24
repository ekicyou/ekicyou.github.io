=begin
  Recent - store recent report/messages information.
  
  Copyright(C) 2005 FUKUOKA Tomoyuki.
  
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
  
  $Id: recent.rb,v 1.1.2.2 2005/01/15 06:56:06 fukuoka Exp $
=end

module Kagemai
  class Recent
    def initialize(data_path, max = 20)
      @filename = "#{data_path}/recent"
      @max      = max
      @recent   = []
      
      unless File.exist?(@filename) then
        File.open(@filename, 'w') {|f| f.print("[]")}
      end
    end
    
    def add(project, report, message)
      latest = RecentEntry.create(project, report, message)
      transaction do |file|
        @recent = do_load(file).reject{|entry| entry.uid == latest.uid}
        @recent.unshift(latest)
        
        max = @max > @recent.size ? @recent.size : @max
        do_save(file, @recent[0...max])
      end
    end
    
    def entries()
      transaction(true) do |file|
        @recent = do_load(file)
      end
      @recent
    end
    
    def each()
      entries().each {|entry| yield entry}
    end
    
  private
    def transaction(read_only = false)
      File.open(@filename, 'r+') do |file|
        file.flock(read_only ? File::LOCK_SH : File::LOCK_EX)
        yield file
      end
    end
    
    def do_load(file)
      recent = []
      src    = file.read
      begin
        Util.safe { 
          recent = eval(src, binding, @filename) 
        }
      rescue Exception, SyntaxError
        $stderr.puts $@[0] + ": " + $!
        recent = []
      end
      recent
    end
    
    def do_save(file, recent)
      file.truncate(0)
      file.rewind()
      
      file.puts "[\n"
      recent.each do |entry|
        file.puts "  " + entry.dump + ", "
      end
      file.puts "]\n"
    end
  end
  
  class ProjectRecent < Recent
    def initialize(project, max = 20)
      super(project.data_dir, max)
      @project    = project
    end
    
    alias super_add add
    def add(report, message)
      super_add(@project, report, message)
    end    
  end
  
  class RecentEntry
    def self.create(project, report, message)
      title = "#{project.name}:#{report.id}"
      time  = message.time
      RecentEntry.new(project.id, report.id, message.id, title, time)
    end
    
    def initialize(project_id, report_id, message_id, title, time)
      @project_id = project_id
      @report_id  = report_id
      @message_id = message_id
      @time       = time
      
      @title = title
      @uid   = "#{title}:#{report_id}"
      
      href_param = "?project=#{project_id}&action=view_report&id=#{report_id}\##{message_id}"
      @href  = "#{Mode::GUEST.url}#{href_param}"
    end
    attr_reader :project_id, :report_id, :message_id, :time
    attr_reader :uid
    
    def anchor()
      %Q!<a href="#{@href}">#{@title}</a>!
    end
    
    def dump()
      %Q!#{self.class}.new(#{@project_id.dump}, #{@report_id}, #{@message_id},
                           #{@title.dump},
                           Time.at(#{@time.to_i}))!
    end
  end
end
