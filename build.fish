#!/usr/bin/fish

set fqbn "arduino:sam:arduino_due_x"
set port "/dev/ttyACM0"
set path "$(pwd)"

function build -a project
	# compile
	arduino-cli compile --fqbn $fqbn $project

	# in case compilation fails, stop
	if test $status -ne 0
		echo "Compilation failed"
		return 1
	else
		echo "Compilation successed!"
	end

	# upload
	sudo arduino-cli upload \
		-p $port \
		--fqbn $fqbn \
		$path/$project

	# in case upload fails, stop
	if test $status -ne 0
		echo "Upload failed"
		return 1
	else
		echo "Upload successed!"
	end

	# monitor
	sudo arduino-cli monitor -p $port
end

#build examples/Bluetooth
#build examples/Fastled
build LedStrip
