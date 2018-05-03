
import os
import sys
import glob
import subprocess

HOME = os.path.dirname(os.path.realpath(__file__))
pathEnv = os.getenv('PATH')
os.environ['PATH'] = "%s" %(HOME) + ":" + pathEnv 


class AudioClip:
	def __init__(self):
		self.num = 0
		self.filename = []
		self.start_time = []
		self.end_time = []

	def put_file(self, name):
		if not (name in self.filename):
			self.filename.append(name)
			self.start_time.append(0.0)
			self.end_time.append(0.0)
			self.num = self.num + 1
		return self.filename.index(name)

	def max_length(self):
		return max(self.end_time)

	def print_filename(self):
		str = ""
		for i in range(self.num):
			str = str + ("-i %s " % self.filename[i])
		return str

	def print_filter(self):
		str = ""
		allch = ""
		for i in range(self.num):
			tmp = "[%d]adelay=%d[ad%d];" % ( (i), int(self.start_time[i]*1000)+1, (i))
			allch = allch + ("[ad%d]" % i)
			str = str + tmp
		str = str + ("%s amix=inputs=%d:dropout_transition=0.5[audio]" % (allch, self.num))
		return str

	def print_audio_info(self, i):
		print "Audio Clip %d: %s: start_time=%.3f, end_time=%.3f" % (i, self.filename[i], self.start_time[i], self.end_time[i])

	def print_ffmpeg(self, output_file):
		if self.num > 1:
			str = "ffmpeg " + self.print_filename()
			str = str + "-filter_complex \"%s\" " % self.print_filter()
			str = str + "-map \"[audio]\" -to %f -y %s" % (self.max_length(), output_file)
		elif self.num == 1:
			str = "ffmpeg -i %s -c:a copy %s" % (self.filename[0], output_file)
		else:
			str = ""
		return str

class VideoClip:
	def __init__(self):
		self.num = 0
		self.filename = []
		self.start_time = []
		self.end_time = []
		self.width = []
		self.height = []
		self.audio_file = ""
		self.audio_start_time = 0.0
		self.audio_end_time = 0.0

	def put_file(self, name):
		if not (name in self.filename):
			self.filename.append(name)
			self.start_time.append(0.0)
			self.end_time.append(0.0)
			self.width.append(0)
			self.height.append(0)
			self.num = self.num + 1
		return self.filename.index(name)
	
	def max_resolution(self):
		self.max_width = max(self.width)
		self.max_height = max(self.height)
		return self.max_width, self.max_height
	
	def max_length(self):
		return max(max(self.end_time), self.audio_end_time)
	
	def audio_delay_needed(self):
		return self.audio_file != "" and self.audio_start_time > 0.05
	
	def print_filter(self):
		if self.audio_delay_needed():
			audio_delay = int(self.audio_start_time*1000)
			str = "[0]adelay=%d[audio];" % audio_delay
		else:
			str = ""
		source = "1"
		sink = "out2"
		for i in range(self.num):
			sink = "out%d" % (i+2)
			if i == self.num - 1:
				sink = "video"
			tmp = "[%d]scale=%dx%d,setpts=PTS-STARTPTS+%.3f/TB[scale%d];[%s][scale%d]overlay=eof_action=pass[%s];" % \
				( (i+2), self.max_width, self.max_height, self.start_time[i], (i+2), source, (i+2), sink )
			str = str + tmp
			source = sink
		return str[:-1]
	
	def print_filename(self):
		str = ""
		for i in range(self.num):
			str = str + ("-i %s " % self.filename[i])
		return str
	
	def print_ffmpeg(self, output_file):
		if self.audio_file == "":
			str = "ffmpeg -f lavfi -i anullsrc "
		else:
			str = "ffmpeg -i %s " % self.audio_file
		str = str + "-f lavfi -i \"color=black:s=%dx%d:r=15\" " % (self.max_width, self.max_height)
		str = str + self.print_filename()
		str = str + "-filter_complex \"%s\" " % self.print_filter()
		if self.audio_file == "":
			map_option = "-map \"[video]\""
		else:
			if self.audio_delay_needed():
				map_option = "-map \"[audio]\" -map \"[video]\" -c:a aac"
			else:
				map_option = "-map 0:a:0 -map \"[video]\" -c:a copy"
		str = str + "%s -c:v libx264 -preset veryfast -to %f -y %s" % (map_option, self.max_length(), output_file)
		return str
	
	def print_audio_info(self):
		print "Audio Clip: %s: start_time=%.3f, end_time=%.3f" % (self.audio_file, self.audio_start_time, self.audio_end_time)
	
	def print_video_info(self, i):
		print "Video Clip %d: %s: start_time=%.3f, end_time=%.3f, width=%d, height=%d" % \
			(i, self.filename[i], self.start_time[i], self.end_time[i], self.width[i], self.height[i])
	

if len(sys.argv) <= 3:
    print "Usage: python tests.py in_path out_file conf_file"
    quit()

in_path = sys.argv[1]
out_file = sys.argv[2]
conf_file = sys.argv[3]

print "in_path %s out_file %s conf_file %s." % (in_path, out_file, conf_file)

# if not os.path.isfile(conf_file):
#     print "conf_file "+conf_file+" dost not exit"
#     quit()

os.chdir(in_path)
child_env = os.environ.copy()
uid_file = conf_file
uid = os.path.splitext(uid_file)[0][4:]
print "UID:"+uid

video_clip = VideoClip()
audio_clip = AudioClip()

with open(uid_file) as f:
    for line in f:
        items = line.split(" ")
        # audio file
        if items[1][-3:] == "aac":
            index = audio_clip.put_file(items[1])
            if items[2] == "create":
                audio_clip.start_time[index] = float(items[0])
            elif items[2] == "close":
                audio_clip.end_time[index] = float(items[0])
        
        # video file (mp4)
        if items[1][-3:] == "mp4":
            index = video_clip.put_file(items[1])
            if items[2] == "create":
                video_clip.start_time[index] = float(items[0])
            elif items[2] == "info":
                video_clip.start_time[index] = float(items[0])
                video_clip.width[index] = int(items[3][6:])
                video_clip.height[index] = int(items[4][7:])
                rotation = int(items[5][9:])
                if rotation == 90 or rotation == 270:
                    video_clip.width[index], video_clip.height[index] = video_clip.height[index], video_clip.width[index]
            elif items[2] == "close":
                video_clip.end_time[index] = float(items[0]) 

        # video file (webm)
        if items[1][-4:] == "webm":
            index = video_clip.put_file(items[1])
            if items[2] == "create":
                video_clip.start_time[index] = float(items[0])
            elif items[2] == "info":
                video_clip.start_time[index] = float(items[0])
                video_clip.width[index] = int(items[3][6:])
                video_clip.height[index] = int(items[4][7:])
                rotation = int(items[5][9:])
                if rotation == 90 or rotation == 270:
                    video_clip.width[index], video_clip.height[index] = video_clip.height[index], video_clip.width[index]
            elif items[2] == "close":
                video_clip.end_time[index] = float(items[0])
    
    video_clip.print_audio_info()
    for i in range(audio_clip.num):
        audio_clip.print_audio_info(i)
    for i in range(video_clip.num):
        video_clip.print_video_info(i)
    
if audio_clip.num > 1:
    print "Generate Audio File"
    tmp_audio = uid+"_tmp.m4a"
    command = audio_clip.print_ffmpeg(tmp_audio)
    video_clip.audio_file = tmp_audio
    video_clip.audio_start_time = 0.0
    video_clip.audio_end_time = audio_clip.max_length()
    print command
    print subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, env=child_env).stdout.read()
elif audio_clip.num == 1:
    print " audio clip num 1"
    video_clip.audio_file = audio_clip.filename[0]
    video_clip.audio_start_time = audio_clip.start_time[0]
    video_clip.audio_end_time = audio_clip.end_time[0]

if video_clip.num > 0:
    print "Generate MP4 file:"
    print "Output resolution:", video_clip.max_resolution()
    output_file = out_file
    print "`````:"+output_file
    command = video_clip.print_ffmpeg(output_file)
else:
    tmp_audio = uid+"_tmp.m4a"
    output_file = uid+".m4a"
    if audio_clip.num > 1:
        command = "mv %s %s" % (tmp_audio, output_file)
    elif audio_clip.num == 1:
        command = "ffmpeg -i %s -c:a copy -y %s" %(video_clip.audio_file, output_file)
print command
print subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, env=child_env).stdout.read()    
print "\n\n"

# remove tmp files
os.system('rm -rf *_tmp.m4a')