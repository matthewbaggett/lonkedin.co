<?php

class mangler{
  public function run(){
    $this->fetchLinkedIn();
  }

  private function fetchLinkedIn(){
    krumo($_SERVER);
    krumo($_REQUEST);
  }
}