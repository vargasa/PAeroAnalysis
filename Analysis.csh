#!/bin/csh 
cd /group/fpi/vargasa/Software/hcana/
source setup.csh /group/fpi/vargasa/Software/hcana/
source /site/12gev_phys/production.csh 2.1
alias hcana /group/fpi/vargasa/Software/hcana/hcana
cd /group/fpi/vargasa/PAeroAnalysis
foreach x ( 464 488 929 )
        hcana -b -q -l "SignalvsYPosition.C($x)"
end
