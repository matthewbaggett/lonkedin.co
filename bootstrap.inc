<?php
chdir(dirname(__FILE__));

error_reporting(E_ALL | E_STRICT);  // Everything but Strict...

require_once("lib/krumo/class.krumo.php");
require_once("lib/simple-html-dom/simple_html_dom.php");
require_once("mangler.inc");
require_once("curl.inc");