=begin
  AddMessage - レポートの追加

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

  $Id: add_message.rb,v 1.1.1.1 2004/07/06 11:44:36 fukuoka Exp $
=end

require 'kagemai/cgi/action'
require 'kagemai/util'
require 'kagemai/message_bundle'
require 'kagemai/cgi/attachment_handler'
require 'kagemai/cgi/form_handler'

module Kagemai
  class AddMessage < Action
    include AttachmentHandler
    include FormHandler

    def execute()
      init_project()

      report_id = @cgi.get_param('id', '')
      raise ParameterError, 'report id not specified.' if report_id.empty?
      
      init_form_handler()
      unless check_message_form(@project.report_type) then
        param = {
          :project    => @project,
          :report_id  => report_id,
          :params     => @cgi,
          :use_cookie => cookie_allowed?,
          :errors     => FormErrors.new(@errors),
          :email_notification => @email_notification
        }

        title = MessageBundle[:title_add_message_e]
        body = eval_template('message_form.rhtml', param)
        return ActionResult.new(title, header(), body, footer(), 
                                @css_url, @lang, @charset)
      end

      # get last message
      report_id = Util.untaint_digit_id(report_id)
      last_message = @project.load_report(report_id).last

      # add new message
      message = Message.new(@project.report_type)
      attachments = {}
      @project.report_type.each do |etype|
        unless etype.kind_of?(FileElementType)
          default = etype.report_attr ? last_message[etype.id] : etype.default
          message[etype.id] = @cgi.get_param(etype.id, default)
        else
          attachment = make_attachment(etype.id)
          if attachment then
            attachments[etype.id] = attachment
          end
        end
      end
      message.set_option('email_notification', email_notification_allowed?)

      store_attachments(@project, message, attachments)
      
      report = @project.add_message(report_id, message)
      title = MessageBundle[:title_add_message]

      param = {:report => report, :message => message}
      body = eval_template('add_message.rhtml', param)
      
      action_result = ActionResult.new(title, 
                                       header(), 
                                       body, 
                                       footer(), 
                                       @css_url, 
                                       @lang, 
                                       @charset)
      handle_cookies(action_result)
    end

    def self.name()
      'add_message'
    end
  end
end
