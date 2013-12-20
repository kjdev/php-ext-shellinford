--TEST--
phpinfo() displays shellinford info
--FILE--
<?php
phpinfo();
?>
--EXPECTF--
%a
shellinford

Shellinford support => enabled
Extension Version => %d.%d.%d
%a
