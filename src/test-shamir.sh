#!/bin/bash

###################################################################
# Copyright (c) Members of the EGEE Collaboration. 2004. See
# http://www.eu-egee.org/partners/ for details on the copyright holders.
# 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#
# Instantiates the configuration templates of this package.
#
# Author: Akos.Frohner@cern.ch
# Author: Joni.Hahkala@cern.ch
#
###################################################################

#export GLITE_SSSS_LOG_LEVEL=DEBUG

# exit after the first problem
set -e

KEY32=$(./glite-ssss-generate-key 32)
./test-shamir 5 2 $KEY32

KEY8=$(./glite-ssss-generate-key 8)
./test-shamir 7 3 $KEY8

echo ""
echo "Testing glite-ssss-split-key and glite-ssss-join-key"
cmd="./glite-ssss-split-key -q 5 2 $KEY32"
echo $cmd
SPLIT32=$($cmd)
cmd="./glite-ssss-join-key -q $SPLIT32"
echo $cmd
JOIN32=$($cmd)
echo "Joined shares: $JOIN32"
if [ "$KEY32" != "$JOIN32" ]; then
    echo "Error: split-join failed!"
    exit -1
fi

