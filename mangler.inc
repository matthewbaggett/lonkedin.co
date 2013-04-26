<?php

class mangler{

  private $linked_in_url;
  private $html;
  /**
   * @var curl_wrapper
   */
  private $curl;
  private $simple_dom;
  private $page;
  private $cache_file;

  private $replacements = array(
    'LinkedIn' => 'LonkedIn',
    'Human' => array('Minion', 'Underling'),
    'employees' => array("Minions", "Underlings", "Gimps", "Winged Monkeys"),
    'Greater Manchester,' => 'Greater Manchester, The Frozen Wastes of the North,',
    'Get introduced to' => 'Make a date with',
    'Consultant' => 'Bullshitter',
    'Team Member' => "Team Gimp",
    'Ruby' => array("Ghetto", "Hipster"),
    'SharePoint' => 'ShitPoint',
    'Scrum' => 'Bum',
    '.Net' => '.Hate',
  );

  public function run(){
    $this->init();
    $this->fetchLinkedIn();
    $this->mangle();
    $this->render();
  }

  private function init(){
    $this->curl = new curl_wrapper();
    $this->simple_dom = new simple_html_dom();
  }

  private function fetchLinkedIn(){
    $this->linked_in_url = "http://www.linkedin.com/" . str_replace("public/","",$_SERVER['REQUEST_URI']);

    $this->cache_file = "cache/" . hash("sha1",$this->linked_in_url).".cache";
    if(!file_exists($this->cache_file)){
      $this->curl->request($this->linked_in_url);
      $this->html = $this->curl->getHttpBody();
      file_put_contents($this->cache_file, $this->html);
    }else{
      $this->html = file_get_contents($this->cache_file);
    }

    $this->page = $this->simple_dom->load($this->html);
  }

  private function mangle(){
    $this->replace($this->page->find('title', 0));
    $this->replace($this->page->find('dd.industry'));
    $this->replace($this->page->find('.join-linkedin ul li'));
    $this->replace($this->page->find('.profile-header .headline-title'));
    $this->replace($this->page->find('#overview .summary-past ul li'));
    $this->replace($this->page->find('#profile-experience .position .postitle .title'));
    $this->replace($this->page->find('#profile-experience .position .orgstats'));
    $this->replace($this->page->find('#profile-skills ol.skills .competency .jellybean'));
    $this->replace($this->page->find('#profile-additional .affiliation .group-data .org'));

    $this->page->find('#footer #copyright')->innertext = "LinkedIn is &copy; LinkedIn and has nothing to do with LonkedIn. LonkedIn was <a href=\"http://twitter.com/matthewbaggett\">Matt's bad idea</a>";
    $this->page->find('ul#nav-legal')->plaintext = '';

    // Get the HTML from simple_dom
    ob_start();
    echo $this->page;
    $this->html = ob_get_contents();
    ob_end_clean();

    // Mangle URLs...
    $this->html = str_replace("http://linkedin.com",      "http://lonkedin.co", $this->html);
    $this->html = str_replace("http://www.linkedin.com",  "http://lonkedin.co", $this->html);
    $this->html = str_replace("https://linkedin.com",     "http://lonkedin.co", $this->html);
    $this->html = str_replace("https://www.linkedin.com", "http://lonkedin.co", $this->html);

  }

  private function replace(&$string){
    if(is_array($string)){
      foreach($string as &$sub_string){
        $this->replace($sub_string);
      }
    }else{
      foreach($this->replacements as $original => $replacement){
        if(is_array($replacement)){
          $replacement_text = $replacement[rand(0,count($replacement)-1)];
        }else{
          $replacement_text = $replacement;
        }
        $string->innertext = str_ireplace($original, $replacement_text, $string->innertext);
      }
    }
  }

  private function render(){
    echo $this->html;
  }
}