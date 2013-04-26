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
  }

  private function init(){
    $this->curl = new curl_wrapper();
    $this->simple_dom = new simple_html_dom();
  }

  private function fetchLinkedIn(){
    $this->linked_in_url = "http://www.linkedin.com/" . str_replace("public/","",$_SERVER['REQUEST_URI']);

    krumo($this->linked_in_url);

    $this->curl->request($this->linked_in_url);

    $this->html = $this->curl->getHttpBody();

    $this->page = $this->simple_dom->load($this->html);

    krumo($this->html);

    krumo($this->page);
  }
}