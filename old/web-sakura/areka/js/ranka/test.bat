@echo off
v8cgi -c .\v8cgi.conf ranka.js  < test_req.txt | iconv -f utf-8 -t cp932 > test_res.txt
type test_res.txt
