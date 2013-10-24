=begin
  Kconv - Kagemai Kanji converter.

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

  $Id: kconv.rb,v 1.4.2.2 2005/01/26 13:47:24 fukuoka Exp $
=end

require 'nkf'

begin
  require 'iconv'
  class Iconv
    def support_utf8?()
      true
    end
  end
rescue LoadError
end

module Kagemai
  module Kconv
    JIS  = 'J'
    SJIS = 'S'
    EUC  = 'e'
    AUTO = ''
    
    def kconv(str, out_code, in_code = AUTO)
      opt = '-m0'
      opt << in_code
      opt << out_code.downcase
      NKF::nkf(opt, str)
    end
    module_function :kconv
    
    def tojis(str)
      NKF::nkf('-jm0', str)
    end
    module_function :tojis
    
    def toeuc(str)
      NKF::nkf('-em0', str)
    end
    module_function :toeuc
    
    def tosjis(str)
      NKF::nkf('-sm0', str)
    end
    module_function :tosjis
  end
  
  class IconvFactory
    class KIconv
      MAP = {
        'JIS'      => Kconv::JIS,
        'SJIS'     => Kconv::SJIS,
        'CP932'    => Kconv::SJIS,
        'ShiftJIS' => Kconv::SJIS,
        'EUC'      => Kconv::EUC,
        'EUC-JP'   => Kconv::EUC
      }
      
      def initialize(to, from) 
        @to   = MAP[to.upcase]
        @from = MAP[from.upcase]
      end
      
      def iconv(text) 
        if @to && @from && @to != @from then
          Kconv.kconv(text, @to, @from)
        else
          text
        end
      end
      
      def support_utf8?()
        false
      end
    end
    
    def self.create(to, from, strict = false)
      if defined?(Iconv) then
        from != to ? Iconv.new(to, from) : KIconv.new(to, from)
      elsif strict && (to == 'UTF-8' || from == 'UTF-8') then
        raise Error, "cannot load iconv library."
      else
        KIconv.new(to, from)
      end
    end
  end
end

class String
  def kkconv(out_code, in_code = Kagemai::KConv::AUTO)
    Kagemai::Kconv::kconv(self, out_code, in_code)
  end
end
