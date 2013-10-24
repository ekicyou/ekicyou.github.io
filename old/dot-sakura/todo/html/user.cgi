#!/usr/bin/env ruby
=begin
  user.cgi -- KAGEMAI CGI Interface (user mode).
  $Id: user.cgi,v 1.1.1.1 2004/07/06 11:44:34 fukuoka Exp $
=end

load 'guest.cgi'
execute(Kagemai::Mode::USER)
