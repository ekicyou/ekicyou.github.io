=begin
  admin.rb - 管理者用のトップページを作成します

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

  $Id: admin.rb,v 1.1.1.1 2004/07/06 11:44:36 fukuoka Exp $  
=end

require 'kagemai/mode'
require 'kagemai/error'
require 'kagemai/cgi/action'
require 'kagemai/message_bundle'
require 'kagemai/cgi/htmlhelper'
require 'kagemai/cgi/form_handler'

module Kagemai
  class AdminPage < Action
    include AdminAuthorization

    def execute()
      check_authorization()
      body = eval_template('admin.rhtml', {:mode => @mode})
      ActionResult.new(MessageBundle[:title_admin], 
                       header(), 
                       body, 
                       footer(), 
                       @css_url, 
                       @lang, 
                       @charset)
    end

    def self.name()
      'admin'
    end

    def self.href(base_url, project_id)
      MessageBundle[:action_admin].href(base_url, {'action' => name()})
    end
  end
end
