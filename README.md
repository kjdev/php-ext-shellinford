# PHP Extension for shellinford (FM-Index)

This extension allows to access the functionality provided by
shellinford (FM-Index).

More information about the shellinford can be found at
[» https://code.google.com/p/shellinford/](https://code.google.com/p/shellinford/)

## Build

```
% phpize
% ./configure
% make
% make install
```

## Configration

shellinford.ini:

```
extension=shellinford.so
```

## Examples

```php
namespace Shellinford;

$docs = array("apple", "orange", "remon", "application");

$index = new FMindex;

foreach ($docs as $doc) {
    $index->push_back($doc);
}

$index->build();

var_dump($index->docsize());
/*
int(4)
*/

var_dump($index->search("app"));
/*
array(2) {
  [0]=>
  string(5) "apple"
  [3]=>
  string(11) "application"
}
*/

var_dump($index->search("apple"));
/*
array(1) {
  [0]=>
  string(5) "apple"
}
*/

var_dump($index->search("apply"));
/*
array(0) {
}
*/
```


## Related

* [api document](http://api.at-ninja.jp/php-ext-shellinford/)
