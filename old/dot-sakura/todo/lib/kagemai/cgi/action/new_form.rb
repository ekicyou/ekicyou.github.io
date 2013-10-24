=begin
  NewForm - レポート作成フォームを作成します。

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

  $Id: new_form.rb,v 1.1.1.1 2004/07/06 11:44:36 fukuoka Exp $  
=end

require 'kagemai/cgi/action'
require 'kagemai/cgi/htmlhelper'
require 'kagemai/cgi/form_handler'

module Kagemai
  class NewForm < Action
    include FormHandler

    def execute()
      init_project()

      param = {
        :cgi                => @cgi,
        :project            => @project,
        :errors             => FormErrors.new,
        :email_notification => @email_notification,
        :allow_cookie       => @use_cookie
      }

      body = eval_template('new_form.rhtml', param)
      ActionResult.new(MessageBundle[:title_new_report], 
                       header(), 
                       body, 
                       footer(), 
                       @css_url, 
                       @lang,
                       @charset)
    end

    def self.name()
      'new_form'
    end

    def self.href(base_url, project_id)
      param = {'action' => name(), 'project' => project_id}
      project_id ? MessageBundle[:action_new_report].href(base_url, param) : nil
    end
  end

end
