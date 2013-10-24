=begin
  NewReport - レポートの新規作成のための処理を行います。

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

  $Id: new_report.rb,v 1.1.1.1 2004/07/06 11:44:36 fukuoka Exp $  
=end

require 'kagemai/cgi/action'
require 'kagemai/cgi/attachment_handler'
require 'kagemai/cgi/form_handler'

module Kagemai
  class NewReport < Action
    include AttachmentHandler
    include FormHandler

    def execute()
      init_project()
      init_form_handler()

      unless check_message_form(@project.report_type) then
        param = {
          :cgi                => @cgi,
          :project            => @project,
          :errors             => FormErrors.new(@errors),
          :email_notification => email_notification_allowed?,
          :allow_cookie       => cookie_allowed?
        }

        body = eval_template('new_form.rhtml', param)
        action_result = ActionResult.new(MessageBundle[:title_new_report_e], 
                                         header(), 
                                         body, 
                                         footer(), 
                                         @css_url, 
                                         @lang,
                                         @charset)
        return action_result
      end

      # create first message of new report.
      message = Message.new(@project.report_type)
      attachments = {}
      @project.report_type.each do |etype|
        unless etype.kind_of?(FileElementType)
          message[etype.id] = @cgi.get_param(etype.id, etype.default)
        else
          attachment = make_attachment(etype.id)
          if attachment then
            attachments[etype.id] = attachment
          end
        end
      end
      message.set_option('email_notification', email_notification_allowed?)
      
      store_attachments(@project, message, attachments)
      report = @project.new_report(message)
      body = eval_template('new_report.rhtml', {:report => report, :message => message})
      
      action_result = ActionResult.new(MessageBundle[:title_new_report_added], 
                                       header(), 
                                       body, 
                                       footer(), 
                                       @css_url, 
                                       @lang,
                                       @charset)
      
      handle_cookies(action_result)
    end

    def self.name()
      'new_report'
    end
  end
end
