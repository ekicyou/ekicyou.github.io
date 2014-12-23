#!../v8cgi

//system.stdout(Util.serialize(system.env, true));
system.stdout(JSON.stringify(system, null, '  '));
var text = "ひ1ら2が3な4";
system.stdout("\n");
system.stdout("ABC\n");
system.stdout(text + "\n");
system.stdout(text.length + "\n");
