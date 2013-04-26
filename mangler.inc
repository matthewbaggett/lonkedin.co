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

  private $replacements = array(
    'LinkedIn' => 'LonkedIn',
    'Human' => array('Minion', 'Underling'),
    'employees' => array("Minions", "Underlings", "Gimps", "Winged Monkeys"),
    'Greater Manchester,' => 'Greater Manchester, The Frozen Wastes of the North,',
    'Get introduced to' => 'Make a date with',
    'consultant' => 'bullshitter',
    'Team Member' => "Team Gimp",
    'Ruby' => array("Ghetto", "Hipster")
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

    $this->curl->request($this->linked_in_url);

    $this->html = $this->curl->getHttpBody();

    $this->page = $this->simple_dom->load($this->html);
  }

  private function mangle(){
    $this->replace($this->page->find('title', 0));
    $this->replace($this->page->find('dd.industry'));
    $this->replace($this->page->find('.join-linkedin ul li'));
    $this->replace($this->page->find('#profile-experience .position .postitle .title'));
    $this->replace($this->page->find('#profile-experience .position .orgstats'));
    $this->replace($this->page->find('#profile-skills ol.skills .competency .jellybean'));
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
    echo $this->page;
  }
}