# libdes
This library implements the Data Encryption Standard as a C89 library.

## Warning!
Even though this implementation might be correct in the _pure_ sense, it is NOT
intended to be used to protect any actual data. The DES algorithm is deprecated
for most usages by world's leading security experts. This implementation is NOT
hardened against any side-channel attacks and is most probably vulnerable.
*Please refrain from using this code for all purposes other than education.*

## License
The `libdes` library is licensed under the terms of the 2-clause BSD
license. For more information please consult the [LICENSE](LICENSE.md)
file. In case you need a different license, feel free to contact the
author.

## Author
Daniel Lovasko (daniel.lovasko@gmail.com)
