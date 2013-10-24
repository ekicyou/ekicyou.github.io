=begin
  ViewReport - レポートの詳細を作成します

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

  $Id: view_report.rb,v 1.1.1.1 2004/07/06 11:44:36 fukuoka Exp $
=end

require 'kagemai/cgi/action'
require 'kagemai/util'
require 'kagemai/message_bundle'
require 'kagemai/cgi/form_handler'

module Kagemai
  class ViewReport < Action
    include FormHandler

    def execute()
      init_project()

      report_id = Util.untaint_digit_id(@cgi.get_param('id'))
      show_form = @cgi.get_param('s', '0') == '1'

      errors = []

      report = @project.load_report(report_id)

      param = {
        :project            => @project,
        :report             => report,
        :email_notification => @email_notification,
        :use_cookie         => @use_cookie,
        :params             => report,
        :show_form          => show_form,
        :errors             => FormErrors.new
      }
      body = eval_template('view_report.rhtml', param)

      ActionResult.new("#{report.id}: #{report['title']}", 
                       header(), 
                       body, 
                       footer(), 
                       @css_url, 
                       @lang,
                       @charset)
    end

    def self.name()
      'view_report'
    end
  end
end
