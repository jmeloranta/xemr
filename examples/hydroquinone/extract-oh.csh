#!/bin/tcsh -f
# Note IHFC will be in MHz

foreach file (*.sps)
  set temp="`echo $file | cut -c10-12`"
  set hfc="`fgrep -i IHFC\ matrix\ \(1\,1\) $file | head -1 | cut -d\  -f1`"
  echo $temp $hfc
end
