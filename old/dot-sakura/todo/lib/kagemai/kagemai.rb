=begin
  kagemai.rb -- KAGEMAI : A Bug Tracking System.

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

  $Id: kagemai.rb,v 1.2.2.6 2005/01/27 13:44:51 fukuoka Exp $
=end

require 'kagemai/config'
require 'kagemai/bts'
require 'kagemai/cgi/action'
require 'kagemai/error'

module Kagemai
  CODENAME = 'Haru'
  VERSION  = '0.8.6'
  URL      = 'http://www.daifukuya.com/kagemai/'
  
  class CGIApplication
    def self.instance()
      # Get applicatoin object from TLS.
      application = Thread.current[:CGIApplication]
      unless application
        raise Error, 'cannnot retrieve CGIApplication object from TLS.'
      end
      return application
    end
    
    def initialize(cgi, mode)
      @cgi = cgi
      @mode = mode
      @bts = BTS.new(Config[:project_dir])
      @actions, @default_action = load_actions(Config[:action_dir])
      @project = nil
      
      @lang = @cgi.get_param('lang', Config[:language])
      MessageBundle.open(Config[:resource_dir], @lang, Config[:message_bundle_name])
      
      # initialize TLS
      Thread.current[:element_renderer] = {}
      
      # Store application object to TLS.
      Thread.current[:CGIApplication] = self
      CGIApplication.instance()
    end
    attr_reader :cgi, :mode, :bts
    
    def action()
      name = @cgi.get_param('action', '')
      action_class = name.empty? ? @default_action : @actions[name]
      if action_class then
        action = action_class.new(@cgi, @bts, @mode, @lang)
        project = action.project
        
        type = action.cache_type
        key = @mode.url + ':' + action.cache_key
        
        if project && Config[:use_html_cache] then
          result = project.load_cache(type, key)
        end
        result = action.execute() unless result
        
        if project then
          if Config[:use_html_cache] then
            project.save_cache(type, key, result)
          else
            project.invalidate_cache(type, key)
          end
        end
        
        result
      else
        raise ParameterError, "No such action : #{name}"
      end
    end
    
    Z_SPACE = "\241\241"
    def cross_search(keyword, case_insensitive = true, 
                     ttype = 'all', projects = [],
                     limit = 50, offset = 0, order = 'report_id')
      results = {}
      total   = 0
      
      keywords = keyword.split(/[\s#{Z_SPACE}]+/oe)
      
      attr_cond = NullSearchCond.new(true)
      @bts.each_project do |project|
        if ttype == 'all' || projects.include?(project.id) then
          result = do_search(project, keywords, attr_cond, case_insensitive, limit, offset, order)
          results[project.id] = result
          total = total + result.total
        end
      end
      [total, results]
    end
    
    private
    @@load_mutex = Mutex.new
    
    def load_actions(dir)
      @@load_mutex.synchronize {
        # load actions
        Dir.glob(dir + "/*\.rb").each do |action_script|
          require action_script.untaint
        end
        
        # init action map
        actions = Hash.new
        default = nil
        ObjectSpace.each_object(Class) do |class_obj|
          ancestors = class_obj.ancestors
          ancestors.delete(class_obj)
          if ancestors.include?(Kagemai::Action) then
            actions[class_obj.name] = class_obj
            default = class_obj if class_obj.default?
          end
        end        
        raise InitializeError, 'No default action loaded.' unless default
        [actions, default]
      }
    end
    
    def do_search(project, keywords, attr_cond, case_insensitive, limit, offset, order)
      search_elements = Hash.new(false)
      
      condition = SearchCondOr.new
      project.report_type.each do |etype|
        search_elements[etype.id] = true
        if keywords.size > 1 then
          acond = SearchCondAnd.new
          keywords.each do |k|
            acond.and(SearchInclude.new(etype.id, k, case_insensitive))
          end
        else
          acond = SearchInclude.new(etype.id, keywords[0], case_insensitive)
        end
        condition.or(acond)
      end
      project.search(attr_cond, condition, true, limit, offset, order)
    end
    
  end
end