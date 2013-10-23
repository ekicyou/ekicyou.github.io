=begin
  PMutex - プロセス/スレッド間での排他制御を提供します。

  Copyright(C) 2002-2004 FUKUOKA Tomoyuki.

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

  $Id: pmutex.rb,v 1.1.1.1.2.1 2005/01/13 19:36:35 fukuoka Exp $
=end

require 'thread'
require 'sync'

module Kagemai

  class PMutex
    SH = Sync::SH
    EX = Sync::EX
    
    @@mutex = Mutex.new
    @@sync = Hash.new
    
    def self.get_sync(filename)
      @@mutex.synchronize {
        unless @@sync.has_key?(filename) then
          @@sync[filename] = [0, Sync.new]
        end
        @@sync[filename][0] += 1
        @@sync[filename][1]
      }
    end
    
    def self.release_sync(filename)
      @@mutex.synchronize {
        @@sync[filename][0] -= 1
        if @@sync[filename][0] == 0 then
          @@sync.delete(filename)
        end
      }
    end
    
    def initialize(lockfile)
      @lockfilename = lockfile
    end
    
    def open_mode(mode)
      r = (mode == SH ? 'rb' : 'wb')
      r
    end
    
    def flock_mode(mode)
      r = (mode == SH ? File::LOCK_SH : File::LOCK_EX)
      r
    end
    
    def lock(mode = EX)
      sync = PMutex.get_sync(@lockfilename)
      sync.lock(mode)
      lockfile = File.open(@lockfilename, open_mode(mode))
      lockfile.flock(flock_mode(mode))
      Thread.current[:sync]     = sync
      Thread.current[:lockfile] = lockfile
    end
    
    def unlock(mode = EX)
      sync     = Thread.current[:sync]
      lockfile = Thread.current[:lockfile]
      
      lockfile.flock(File::LOCK_UN)
      lockfile.close()
      sync.unlock(mode)
      PMutex.release_sync(@lockfilename)
    end
    
    def synchronize(mode = EX)
      begin
        lock(mode)
        yield
      ensure
        unlock(mode)
      end
    end

  end

end
