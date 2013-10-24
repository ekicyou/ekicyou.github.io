#!/usr/bin/env ruby
=begin
  admin.cgi -- KAGEMAI CGI Interface (administrator mode).
  $Id: admin.cgi,v 1.1.1.1 2004/07/06 11:44:34 fukuoka Exp $
=end

load 'guest.cgi'
execute(Kagemai::Mode::ADMIN)
