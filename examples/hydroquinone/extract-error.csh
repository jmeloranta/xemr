#!/bin/tcsh -f

foreach file (*.sps)
  set temp="`echo $file | cut -c10-12`"
  set err="`fgrep -i rms $file | cut -d\  -f1`"
  set scale="`fgrep -i Amplitude\ for\ spectrum\ \#1 $file | cut -d\  -f1`"
  set err = "`echo $err / $scale | bc`"
  echo $temp $err
end
