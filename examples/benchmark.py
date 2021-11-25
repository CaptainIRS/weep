import subprocess

def run_simulation(totalTime, range, dataStart, nodes, sinks, speed, scheduler):
    subprocess.call([
        './waf',
        '--run',
        f'weep-wifi-sim \
            --TotalTime={totalTime} \
            --Range={range} \
            --DataStart={dataStart} \
            --Nodes={nodes} \
            --Sinks={sinks} \
            --Speed={speed} \
            --Scheduler={scheduler}',
        '--cwd',
        'contrib/weep/outputs/'
    ])



totalTime = 100.0
range = 10.0
dataStart = 1.0
sinks = 2
speed = 5
scheduler = "ns3::weep::AodvWeepScheduler"

for nodes in [5, 10, 15, 20]:
    run_simulation(totalTime, range, dataStart, nodes, sinks, speed, scheduler)