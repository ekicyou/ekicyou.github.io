#!/usr/bin/env ruby
=begin
  guest.cgi - KAGEMAI CGI main
  
  Copyright(C) 2002-2005 FUKUOKA Tomoyuki, DAIFUKUYA.
  
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
  
  $Id: guest.cgi,v 1.3.2.8 2005/01/15 14:10:53 fukuoka Exp $
=end

BEGIN { $defout.binmode }

$KCODE = 'e'
$SAFE = 1

# $DEBUG = 1
$SHOW_ENV_VARS = false # debug
$KAGEMAI_DEBUG = false # debug

work_dir = File.dirname(File.expand_path(__FILE__)).untaint # setup
if File.symlink?(work_dir) then
  work_dir = File.readlink(work_dir)
end

kagemai_root = File.dirname(work_dir.untaint) # setup
config_file  = work_dir + '/kagemai.conf' # setup

$:.unshift(kagemai_root + '/lib')

require 'kagemai/config'

Kagemai::Config.initialize(kagemai_root, config_file)

require 'kagemai/kagemai'
require 'kagemai/mode'
require 'kagemai/kcgi'

if $KAGEMAI_DEBUG then
  ## init Logger for debugging
  require 'kagemai/logger'
  Kagemai::Logger.level = Kagemai::Logger::DEBUG
  Kagemai::Logger.add_category('Temp')
end

def print_maintenance_message()
  print CGI.new.header({'status' => 'OK', 'type' => 'text/plain'})
  puts 'This system is under maintenance now.'
  puts 'Please visit again several hours later.'
  puts '--'
  puts 'Bug Tracking System KAGEMAI.'
end

def execute(mode)
  cgi = nil
  
  begin
    if Kagemai::Config[:maintenance_mode] && mode != Kagemai::Mode::ADMIN then
      print_maintenance_message()
      return
    end
    
    cgi = Kagemai::KCGI.new("html4Tr")
    app = Kagemai::CGIApplication.new(cgi, mode)
    
    result = app.action()
    result.respond(cgi, $KAGEMAI_DEBUG, $SHOW_ENV_VARS)
    
  rescue Kagemai::Error => e
    cgi = CGI.new("html4Tr") unless cgi
    
    err_msg = '<p class="error">Following errors occurred.</p>'
    err_msg += "\r\n<pre>#{e.class}: #{e.to_s.escape_h}</pre>"
    err_msg += %Q!\r\n<pre>#{e.backtrace.join("\r\n")}</pre>! if $KAGEMAI_DEBUG
    
    header_param = {
      'type' => 'text/html', 
      'charset' => 'EUC-JP', 
      'language' => 'ja'
    }
    
    css_param = {
      'href' => 'kagemai.css', 
      'type' => 'text/css', 
      'rel' => 'stylesheet'
    }
    
    meta_param1 = {
      'http-equiv' => 'Content-Type', 
      'content' => "text/html; charset=EUC-JP"
    }
    
    meta_param2 = {
      'http-equiv' => 'Content-Style-Type', 
      'content' => "text/css"
    }
    
    body = cgi.html() {
      cgi.head { 
        "\r\n" +
        cgi.meta(meta_param1) + "\r\n" + 
        cgi.meta(meta_param2) + "\r\n" + 
        cgi.link(css_param) + "\r\n" + 
        cgi.title{e.class.to_s}
      } + "\r\n" + cgi.body { "\r\n" + err_msg + "\r\n" }
    }
    header_param['length'] = body.size
    
    print cgi.header(header_param)
    print body
  end
  
rescue Exception => e
  print "HTTP/1.1 200 OK\r\n" if defined?(MOD_RUBY)
  print "Content-Type: text/plain\r\n\r\n"
  puts 'Following errors occurred. Please contact administrator.'
  puts ''
  puts "#{e} (#{e.class})"
    
  if $KAGEMAI_DEBUG then
    puts ''
    puts e.backtrace.join("\r\n")
    puts '-------------------------------'
    puts 'Debug Log: '
    puts Kagemai::Logger.buffer()
  end
  
  raise unless $KAGEMAI_DEBUG
end

script_filename = File.basename(ENV.fetch('SCRIPT_FILENAME', $0))
if script_filename == File.basename(__FILE__) then
  execute(Kagemai::Mode::GUEST)
end
