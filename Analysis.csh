#!/bin/tcsh
alias hcana /home/vargasa/Software/hcana/hcana
source /site/12gev_phys/production.csh 2.1
cd /home/vargasa/Software/hcana/
source setup.csh
cd /home/vargasa/PAeroAnalysis/
foreach x ( 464 488 929 )
	hcana -b -q -l "SignalvsYPosition.C($x)" 
end

