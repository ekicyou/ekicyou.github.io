=begin
 MessageBundle - message resource bundle

  Copyright(C) 2002-2004 FUKUOKA Tomoyuki.

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

 $Id: message_bundle.rb,v 1.1.1.1 2004/07/06 11:44:35 fukuoka Exp $
=end

require 'kagemai/error'

module Kagemai
  class MessageBundle
    def self.open(base_dir, lang, filename)
      File.open("#{base_dir}/#{lang}/#{filename}", 'rb') do |file|
        Thread.current[:MessageBundle] = MessageBundle.new(file)
      end
      Thread.current[:MessageBundle]
    end
    
    def self.update(filename)
      File.open(filename, 'rb') do |file|
        Thread.current[:MessageBundle].update(file)
      end
    end
    
    def self.[](key)
      Thread.current[:MessageBundle][key]
    end

    def self.has_key?(key)
      Thread.current[:MessageBundle].has_key?(key)
    end

    def initialize(file)
      @messages = {}
      load_messages(file)
    end

    def update(file)
      load_messages(file)
    end

    def load_messages(file)
      file.each do |line|
        line = line.sub(/#.*/, '').strip()
        next if line.empty?
        
        key, *message = line.split(/=/)
        key = key.to_s.strip
        message = message.join('=').to_s.strip
        next if (key.empty? || message.empty?)

        @messages[key.intern] = message
      end
    end

    def [](key)
      unless @messages.has_key?(key) then
        raise NoSuchResourceError, "No message resource for '#{key.inspect}'"
      end
      @messages[key]
    end

    def has_key?(key)
      @messages.has_key?(key)
    end
  end
end
