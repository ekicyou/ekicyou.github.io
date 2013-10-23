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

  $Id: set_email_notify.rb,v 1.1.1.1.2.1 2004/09/23 21:24:50 fukuoka Exp $  
=end

require 'kagemai/cgi/action'
require 'kagemai/cgi/form_handler'

module Kagemai
  class SetEmailNotification < Action
    include AdminAuthorization
    include FormHandler

    def execute()
      check_authorization()
      init_project()

      report_id = Util.untaint_digit_id(@cgi.get_param('id'))
      report = @project.load_report(report_id)

      report.each do |message|
        if @cgi.get_param(message['email'], '') == 'on' then
          message.set_option('email_notification', true)
        else
          message.set_option('email_notification', false)
        end
      end

      @project.update_report(report)

      param = {
        :project            => @project,
        :report             => report,
        :email_notification => @email_notification,
        :use_cookie         => @use_cookie,
        :params             => report,
        :show_form          => false,
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
      'set_email_notification'
    end
  end
end
