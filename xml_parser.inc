<?php

/**
 * Copyright 2010 Luca Hsieh (viirya)
 * 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 *        
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */
 
class xml_parser {
 
   var $dom;
   var $root;
   var $ret;
   var $encoding;
 
   function convert_attrs($att) {
 
     $ret = array();
 
     foreach($att as $i)
       $ret[$i->name] = $i->value;
 
     return $ret;
 
   }
   function parse_node($node) {
       $ret = '';
       $node_name = $node->nodeName;
       if ($node->hasChildNodes()) {
           foreach($node->childNodes as $n) {
               if ($n->nodeName == '#text' || $n->nodeName == '#cdata-section') {
                   if (!is_array($ret)) {
                     $ret = $n->nodeValue;
                   }
                   $node_value = $n->nodeValue;
               }
               else {
                   if (isset($ret) && !is_array($ret))
                     $ret = array();
                   $tmp = $this->parse_node($n);
                   $attrs = $n->attributes;

                   if ($attrs != NULL) {
                     $attrs = $this->convert_attrs($attrs);
                     if (!empty($attrs)) {
                       $tmp2 = $tmp;
                       $tmp = array();
                       $tmp['value'] = $tmp2;
                       $tmp['_attrs'] = $attrs;
                     }
                   }
                   if (!isset($ret[$n->nodeName])) {
                     $ret[$n->nodeName] = $tmp;
                   }
                   else {
                     if (is_array($ret[$n->nodeName]) && !isset($ret[$n->nodeName][0])) {
                       $switch = $ret[$n->nodeName];
                       $ret[$n->nodeName] = array();
                       $ret[$n->nodeName][0] = $switch;
                     }
                     else if (!is_array($ret[$n->nodeName]) && isset($ret[$n->nodeName])) {
                       $switch = $ret[$n->nodeName];
                       $ret[$n->nodeName] = array();
                       $ret[$n->nodeName][0] = $switch;
                     }
                     $ret[$n->nodeName][] = $tmp;
                   }
               }
           }
       }
       return $ret;
   }
 
   function parse($xml) {

     $this->dom = new DOMDocument();
     $this->dom->loadXML($xml);
     $this->ret = array();

     if (!isset($this->dom) || $this->dom == false)
       return $this->ret;
 
     $this->root = $this->dom->documentElement;
     $this->ret[$this->root->nodeName] = $this->parse_node($this->root);
 
     $this->encoding = '';
     $matches = array();
     if (preg_match('/<\?xml.*encoding="(.*?)"\?>/', $xml, $matches))
       $this->encoding = $matches[1];
 

     return $this->ret;
   }
 
   function get_encoding() {
     return $this->encoding;
   }
 

}

?>

