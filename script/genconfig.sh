
#!/bin/bash

echo $(pwd)
cd build

echo '#!/bin/bash' > cc
echo $1 '$@' >> cc

echo '#!/bin/bash' > cxx
echo $2 '$@' >> cxx

cd ..