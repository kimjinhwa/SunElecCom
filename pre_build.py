import os
import subprocess
Import("env")
def get_git_info():
    try:
        print(env.get("PROJECT_BUILD_DIR"))
        print(env.get("BUILD_DIR"))
        branch_name = subprocess.check_output(['git', 'rev-parse', '--abbrev-ref', 'HEAD']).strip().decode()
        commit_hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).strip().decode()
        pioenv= env.get("PIOENV")
        return branch_name, commit_hash,pioenv
    except Exception as e:
        print("Error retrieving Git information:", str(e))
        return None, None, None
        #env.Replace(PROGNAME="firmware_%s" % env.GetProjectOption("custom_prog_version"))
branch_name, commit_hash, pioenv = get_git_info()
env.Replace(PROGNAME="firmware_%s_%s%s" %  (pioenv,branch_name , commit_hash) )
