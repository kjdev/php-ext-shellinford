--TEST--
search
--SKIPIF--
--FILE--
<?php
namespace Shellinford;

$docs = array("apple", "orange", "remon", "application");

$index = new FMindex;
var_dump($index);

foreach ($docs as $doc) {
    $index->push_back($doc);
}

$index->build();

var_dump($index->docsize());

var_dump($index->search("app"));
var_dump($index->search("apple"));
var_dump($index->search("apply"));

?>
--EXPECTF--
object(Shellinford\FMindex)#%d (0) {
}
int(4)
array(2) {
  [0]=>
  string(5) "apple"
  [3]=>
  string(11) "application"
}
array(1) {
  [0]=>
  string(5) "apple"
}
array(0) {
}
