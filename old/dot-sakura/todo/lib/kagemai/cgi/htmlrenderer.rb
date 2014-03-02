=begin
  Renderer - HTML rendering module

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

  $Id: htmlrenderer.rb,v 1.1.1.1 2004/07/06 11:44:36 fukuoka Exp $
=end

module Kagemai

  module HtmlRenderer
    def initialize(index_only = false)
      @index_only = index_only
    end

    def render(element, value)
      result = value

      renderers().each do|renderer|
        result = renderer.do_render(element, result)
      end

      temporal().each do |renderer|
        result = renderer.do_render(element, result)
      end

      result
    end

    def add_renderer(renderer)
      renderers() << renderer
    end

    def add_renderer_front(renderer)
      renderers().unshift(renderer)
    end

    def remove_renderer(renderer)
      renderers().delete_if {|x| x.equal?(renderer)}
    end

    def add_temporal_renderer(renderer)
      temporal() << renderer
    end

    def remove_temporal_renderers()
      @temporal = []
    end

    def do_render(element, value)
      value
    end

    def index_only?()
      @index_only
    end

    private
    def renderers()
      @renderers ||= [self]
      @renderers
    end

    def temporal()
      @temporal ||= []
      @temporal
    end
  end

  class BtsLinkRenderer
    include HtmlRenderer
    
    def do_render(element, value)
      mode = CGIApplication.instance.mode
      value.gsub(/&lt;(BTS|bts)(?::(#{Project::ID_REGEXP_STR}))?:(\d+)&gt;/m) {
        p_str = ''
        if $2 then
          project_id = $2
          report_id = $3
          p_str = ":#{project_id}"
        else
          project_id = Project.instance.id
          report_id = $3
        end
        param = {
          'action'  => ViewReport.name,
          'project' => project_id,
          'id'      => report_id
        }
        "&lt;#{$1}#{p_str}:#{report_id}&gt;".href(mode.url, param)
      }
    end
  end

  class UrlRenderer
    include HtmlRenderer

    module HttpUrl 
      # RFC 2396, 2616, RubyMagic pp.75-85
      # NOTE: escape_h ���줿ʸ������оݤˤ��뤿�ᡢ'&' �� '&amp;' �ˤ��Ƥ��롣
      #       ���Ѿ� '(' �� ')' �ϴޤ�ʤ������ɤ������ʤΤǺ�����Ƥ��� (BTS:86)
      alpha         = '[A-Za-z]'
      alphanum      = '[\w]'
      alphanum2     = '[\w\-]'
      escaped       = '%[0-9A-Fa-f]{2}'
      reserved      = '(?:[;/?:@=+$,]|&amp;)' # s/&/&amp;/
      unreserved    = '[\w\-_.!~*\']'
      uric          = "(?:#{reserved}|#{unreserved}|#{escaped})"
      query         = "#{uric}*"
      pchar         = "(?:#{unreserved}|#{escaped}|[:@=+$,]|&amp;)" # s/&/&amp;/
      param         = "#{pchar}*"
      segment       = "#{pchar}*(?:;#{param})*"
      path_segments = "#{segment}(?:/#{segment})*"
      abs_path      = "/#{path_segments}"
      port          = '\d*'
      ipv4address   = '\d+\.\d+\.\d+.\d+'
      toplabel      = "#{alpha}(?:#{alphanum2}*#{alphanum})?"
      domainlabel   = "#{alphanum}(?:#{alphanum2}*#{alphanum})?"
      hostname      = "(?:#{domainlabel}\\.)*#{toplabel}\\.?"
      host          = "(?:#{hostname}|#{ipv4address})"
      
      fragment      = "#{uric}*"
      scheme        = "(?:http|https|ftp)"
      
      REGEXP_STR  = "#{scheme}://#{host}(?::#{port})?(?:#{abs_path}(?:\\?#{query})?)?(?:##{fragment})?"
      REGEXP      = Regexp.new(REGEXP_STR)
      REGEXP_LINE = Regexp.new('^' + REGEXP_STR + '$')
    end

    def do_render(element, value)
      value.gsub(/(#{HttpUrl::REGEXP_STR})/m, '\1'.href('\1'))
    end
  end

  require 'kagemai/fold'
  class Folding
    include HtmlRenderer
    
    def initialize(column = nil)
      if column then
        @column = column
      else
        @column = Project.instance.fold_column
      end
    end
    
    def do_render(element, value)
      preserved = []
      lines = value.gsub(/<a.*?>.*?<\/a>/im) {
        preserved << $~
        "\001"
      }
      
      result = lines.collect {|line| 
        (/^([>+\-=!\s]|RCS file:|&gt;)/ =~ line) ? line : Fold::fold_line(line, @column)
      }.join('')
      
      result.gsub(/\001/m) {
        preserved.shift
      }
    end
  end

end