=begin
  CGI - CGI クラスの拡張です

  Copyright(C) 2002, 2003 FUKUOKA Tomoyuki.

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

  $Id: kcgi.rb,v 1.2 2004/08/03 22:15:46 fukuoka Exp $
=end
require 'cgi'
require 'delegate'
require 'tempfile'
require 'kagemai/kconv'
require 'kagemai/config'

module Kagemai
  class KCGI < SimpleDelegator
    def initialize(type = "query", lang = Config[:language], charset = Config[:charset])
      @cgi = CGI.new(type)
      super(@cgi)
      
      init_multipart()

      @kconv_proc = Proc.new {|str| str}
      
      @lang = get_param('lang', lang)
      @charset = get_param('charset', charset)
      
      init_kconv(@lang, @charset)
    end
    attr_reader :lang, :charset
    
    def init_multipart()
      if ENV['REQUEST_METHOD'] == 'POST' then
        @multipart = (/\Amultipart\/form-data/ =~ ENV['CONTENT_TYPE']) != nil
      else
        @multipart = false
      end
      
      if @multipart then
        @params_r = Hash.new
        def self.do_get_param(key)
          unless @params_r.has_key?(key) then
            @params_r[key] = ''
            if @cgi.params[key].size > 0 then
              @params_r[key] = @cgi.params[key].collect{|p| p.read}.join(",\n")
            end
          end
          @params_r[key]
        end
      else
        def self.do_get_param(key)
          @cgi.params[key].join(",\n")
        end
      end
    end

    def init_kconv(lang, charset)
      return if lang.upcase != 'JA'
      return if ENV.fetch('REQUEST_METHOD', 'GET').upcase != 'POST'
      
      @lang = lang
      @charset = charset
      
      @out_code = case charset.upcase
                  when 'EUC-JP'
                    Kconv::EUC
                  when 'SHIFTJIS'
                    Kconv::SJIS
                  when 'ISO-2022-JP'
                    Kconv::JIS
                  when 'SJIS'
                    Kconv::SJIS
                  when 'JIS'
                    Kconv::JIS
                  else
                    Kconv::AUTO
                  end
      
      in_char = do_get_param('jp_enc_test').to_s.strip
      raise ParameterError, 'no jp_enc_test.' if in_char.to_s.empty?
      
      @in_code = case in_char[0]
                 when 0xC6 # EUC-JP
                   Kconv::EUC
                 when 0x93 # SJIS
                   Kconv::SJIS
                 when 0x1B # JIS
                   Kconv::JIS
                 when 0xFD # UTF-8
                   message = "unsupported Japanese encodeing (UTF-8). "
                   raise ParameterError, message
                 else
                   bytes = []
                   in_char.each_byte do |b|
                     bytes << sprintf("0x%02X", b)
                   end
                   message = "unsupported Japanese encodeing.\r\n"
                   message += "jp_enc_test = #{in_char}\r\n"
                   message += "jp_enc_test = [#{bytes.join(', ')}]"
                   raise ParameterError, message
                 end
      
      @kconv_proc = Proc.new{|str|
        str.kkconv(@out_code, @in_code)
      }
    end
    
    def each()
      @cgi.params.each_key do |key|
        yield key, get_param(key)
      end
    end
    
    def params()
      @multipart ? @params_r : @cgi.params
    end
    
    def get_param(key, default = nil)
      v = do_get_param(key).to_s.strip
      v.empty? ? default : @kconv_proc.call(v).gsub(/\r\n/m, "\n").gsub(/\r/m, "\n")
    end
    
    def get_attachment(id)
      io = @cgi.params[id][0]
      
      if (defined? StringIO) && io.kind_of?(StringIO) then
        return nil if io.size == 0
        
        file = Tempfile.new("CGI")
        
        file.binmode
        file.print io.string
        file.rewind
        
        def file.sio=(sio)
          @sio = sio
        end
          
        def file.original_filename()
          @sio.original_filename()
        end
          
        def file.local_path()
          path()
        end
          
        def file.content_type()
          @sio.content_type()
        end
        
        file.sio = io
        io = file
      end
        
      (io && io.stat.size > 0) ? io : nil
    end

    alias :attr :get_param
    alias :fetch :get_param
    
    def mobile_agent?()
      m_agents = [
        'DoCoMo', 'J-PHONE', 'UP\.Browser', 'DDIPOCKET',
        'ASTEL', 'PDXGW', 'Palmscape', 'Xiino',
        'sharp pda browser', 'Windows CE', 'L-mode'
      ]
      self.user_agent =~ /(#{m_agents.join('|')})/i
    end
    
    def ua_ie?()
      self.user_agent =~ /(compatible; MSIE)|(Sleipnir)/
    end
    
    def ua_firefox?()
      self.user_agent =~ /Firefox/
    end
    
    def ua_mozilla?()
      !ua_ie? && !ua_firefox? && self.user_agent =~ /Mozilla\/5.0/
    end
  end
end
