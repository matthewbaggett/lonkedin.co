<?php
if ((!isset($_REQUEST['password']) || $_REQUEST['password'] !== 'kU4EdRaw') && PHP_SAPI !== 'cli') {
  die("Wrong password");
}
chdir("../");
ob_start();

$libraries = array(
  "krumo" => "https://github.com/tony/krumo.git",
  "pretty-exceptions" => "git://github.com/matthewbaggett/pretty-exceptions.git",
  "simple-html-dom" => "git://github.com/jjanyan/Simple-HTML-DOM.git",
);

// The commands
$commands = array(
  'echo $PWD',
  'whoami',
  'hostname',
  'git status',
  'git pull',
  'git status',
);

foreach($libraries as $name => $location){
  if(file_exists("lib/{$name}")){
    $commands[] = "cd lib/{$name}; git pull";
  }else{
    $commands[] = "mkdir lib/{$name} -p; cd lib/{$name}; pwd; git clone {$location} . --verbose";
  }
}

// Run the commands for output
$output = '';
foreach ($commands AS $command) {
  // Run it
  $tmp = shell_exec($command);
  // Output
  $output .= "<span style=\"color: #6BE234;\">\$</span> <span style=\"color: #729FCF;\">{$command}\n</span>";
  $output .= htmlentities(trim($tmp)) . "\n";
}

if(PHP_SAPI !== 'cli'){
  ?>
  <!DOCTYPE HTML>
  <html lang="en-US">
  <head>
    <meta charset="UTF-8">
    <title>GIT DEPLOYMENT SCRIPT</title>
  </head>
  <body style="background-color: #000000; color: #FFFFFF; font-weight: bold; padding: 0 10px;">
<pre>
 .  ____  .    ____________________________
 |/      \|   |                            |
[| <span style="color: #FF0000;">&hearts;    &hearts;</span> |]  | Git Deployment Script v0.1 |
 |___==___|  /              &copy; oodavid 2012 |
              |____________________________|

  <?php echo $output; ?>
</pre>
  </body>
  </html>
  <?php
  $output = ob_get_contents();
  ob_end_clean();
}
echo $output;

$to = "matthew@baggett.me";
// To send HTML mail, the Content-type header must be set
$headers  = 'MIME-Version: 1.0' . "\r\n";
$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
$headers .= 'To: ' . $to . "\r\n";
$headers .= 'From: GitHub AutoDeploy on ' . gethostname() . " <service@gamitu.de>\r\n";

mail(
  $to,
  "Deployment on ".gethostname()." at " . date("d/m/Y H:i:s"),
  $output,
  $headers
);
?>
