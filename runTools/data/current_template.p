(lp0
S'\xef\xbb\xbf#SBATCH -W {walltime_request}\n#SBATCH -o /home/jdecunha/MicroTrackGenerator/run_logfiles\n#SBATCH -cwd /home/jdecunha/MicroTrackGenerator\n#SBATCH -q s\n#SBATCH -n 40\n#SBATCH -u jdecunha@mdanderson.org\n#SBATCH -J {job_name}\n\nsource /home/jdecunha/configure.sh\n\n{run_command}\n'
p1
aS'.sh'
p2
a.