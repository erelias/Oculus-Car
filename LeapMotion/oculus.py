################################################################################
# Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.			   #
# Leap Motion proprietary and confidential. Not for distribution.			   #
# Use subject to the terms of the Leap Motion SDK Agreement available at	   #
# https://developer.leapmotion.com/sdk_agreement, or another agreement		   #
# between Leap Motion and you, your company or other organization.			   #
################################################################################

import Leap, sys, thread, time
import requests
from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture


class SampleListener(Leap.Listener):
	finger_names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
	bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']
	state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']

	def on_init(self, controller):
		print "Initialized"

	def on_connect(self, controller):
		print "Connected"

		# Enable gestures
	
		#controller.enable_gesture(Leap.Gesture.TYPE_SWIPE);
		controller.enable_gesture(Leap.Gesture.TYPE_CIRCLE);
		controller.config.set("Gesture.Swipe.MinLength", 10.0)
		controller.config.set("Gesture.Swipe.MinVelocity", 5)
		controller.config.set("Gesture.Circle.MinRadius", 50)
		controller.config.save()
		
	def on_disconnect(self, controller):
		# Note: not dispatched when running in a debugger.
		print "Disconnected"

	def on_exit(self, controller):
		print "Exited"

	def on_frame(self, controller):
		# Get the most recent frame and report some basic information
		frame = controller.frame()

	  #	 print "Frame id: %d, hands: %d" % (
	   #	   frame.id, len(frame.hands))

		# Get hands
		for gest in frame.gestures():
			if(gest.type == Leap.Gesture.TYPE_CIRCLE):
				print "Go back"
				payload = {'left': 'B140', 'right': 'B140'}
				r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
				import time
				time.sleep(2)
				print "Done going back"
			elif(gest.type == Leap.Gesture.TYPE_SWIPE):
				swipe = SwipeGesture(gest)
				print swipe.length
				if swipe.direction.x < 0:
					print "Look left"
					payload = {'servo': 180}
					r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
				elif swipe.direction.x > 0:
					print "Look right"
					payload = {'servo': 0}
					r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
		pos = [[0,0],[0,0]]
		count = 0
		if len(frame.hands) ==2:
			for hand in frame.hands:

				pos [count][0] = hand.palm_position[0]
				pos [count][1] = hand.palm_position[1]
				count += 1
				
			print pos
			handtype = 0 if hand.is_left else 1
			if pos[handtype][1]/pos[1-handtype][1] > 1.25:
				payload = {'left': "F000", 'right': "F185"}
				r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
				print "Turn left"
			elif pos[1-handtype][1]/pos[handtype][1] > 1.25:
				payload = {'left': "F185", 'right': "F000"}
				r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
				print "Turn right"
			else:
				payload = {'left': "F185", 'right': "F185"}
				r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
				print "Straight"
			#left = 110+int(145*pos[0][1]/(pos[0][1]+ pos[1][1]))
			#right  = 110+int(145*pos[1][1]/(pos[0][1]+ pos[1][1]))
			
			#rint "Right %d, Left %d" %(left, right)
			#leftStr = 'F'+'0'*(3-len(str(left)))+str(left)
			#rightStr = 'F'+'0'*(3-len(str(right)))+str(right)
			#print leftStr, rightStr
			#payload = {'left': leftStr, 'right': rightStr}
			#r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
			
			
		else:
			#print "Please put two hands above leap motion"
			payload = {'left': 'F000', 'right': 'F000'}
			r = requests.post("http://oculuscar.herokuapp.com/", data=payload)
			
		
	def state_string(self, state):
		if state == Leap.Gesture.STATE_START:
			return "STATE_START"

		if state == Leap.Gesture.STATE_UPDATE:
			return "STATE_UPDATE"

		if state == Leap.Gesture.STATE_STOP:
			return "STATE_STOP"

		if state == Leap.Gesture.STATE_INVALID:
			return "STATE_INVALID"

def main():
	# Create a sample listener and controller
	listener = SampleListener()
	controller = Leap.Controller()

	# Have the sample listener receive events from the controller
	controller.add_listener(listener)

	# Keep this process running until Enter is pressed
	print "Press Enter to quit..."
	try:
		sys.stdin.readline()
	except KeyboardInterrupt:
		pass
	finally:
		# Remove the sample listener when done
		controller.remove_listener(listener)


if __name__ == "__main__":
	main()