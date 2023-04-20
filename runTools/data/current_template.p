(lp0
S'#!/bin/bash\n\xef\xbb\xbf\n#BSUB -W {walltime_request}\n#BSUB -o /rsrch3/scratch/radphys_rsch/jdecunha/MicroTrackGenerator/run_logfiles\n#BSUB -cwd /rsrch3/scratch/radphys_rsch/jdecunha/MicroTrackGenerator/runFiles\n#BSUB -q short\n#BSUB -M 165\n#BSUB -R rusage[mem=165]\n#BSUB -n 28\n#BSUB -u jdecunha@mdanderson.org\n#BSUB -J {job_name}\n\nsource /rsrch3/home/radphys_rsch/jdecunha/configure.sh\n\n{run_command}\n'
p1
aS'.lsf'
p2
a.