#!/bin/tcsh -f

foreach file (*.sps)
  set temp="`echo $file | cut -c10-12`"
  set rate="`fgrep -i norris $file | cut -d\  -f1`"
  echo $temp $rate
end
