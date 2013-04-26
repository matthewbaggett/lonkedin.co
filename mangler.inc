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
    $this->page->find('title', 0)->innertext = 'Buttocks.';
  }

  private function render(){
    echo $this->page;
  }
}