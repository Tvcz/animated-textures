# Final Project - Computer Graphics

<img src="./media/header.jpg">

## Description

An important part of any computer graphics course, in my opinion, is having the opportunity to build a project that you care about. The final project in this course showcases your creativity from what you have learned over the semester and can be used as a portfolio piece for future internship, co-op, and full-time job opportunities.  We have covered enough computer graphics now that you are ready to design and create a project from scratch.
  
## Expectations

You can work on any idea you want that is within the scope of graphics APIs we have covered (e.g. OpenGL + C++). You may explore areas that we did not cover heavily in class (Animation, particles, different rendering techniques, advanced shaders, image processing, volume rendering, etc.).  Your final project should be on the order of the difficulty beyond the homeworks.  

## Banned Project Ideas and Guidance (You cannot or otherwise are strongly advised against these ideas)

1. Solar Systems based off of previous Assignments
	- Sorry, I've seen WAY too many, and they tend to underperform as portfolio pieces.
		- (most folks just add more planets which is not enough effort for a final project).
		- A final project should be at least the equivalent effort of an assignment if not more.
2. Pong
	- You have to understand, the bar is not high for implementing the game pong.
	- There is no creativity in this project, employers will not be impressed if this is on your portfolio, it's just not the right thing to implement. 
	- Pong is great for learning, useful for understanding how to structure a game, but just not a final project after a semester of computer graphics.
3. Post processing with lots of trivial kernels
	- I've seen it too often that folks copy previous code and just take the kernels and change them. This is not a deep enough project. You have to create a new kernel, perform image stiching, or do something that is not trivial.
4. Again, **you cannot use a game engine or framework** like Unity3D, Unreal Engine, Godot, pygame, three.js, phaser.js, OGRE, etc. for your project.
	- You must use a low-level graphics API (OpenGL, Vulkan, Metal, WebGL).
		- **You cannot just use SDL2** -- you need to use a low-level graphics API (e.g. OpenGL, maybe Vulkan if instructor approved)
	- Some further guidance:
			- In years past folks have completely avoided OpenGL and just used SDL2 for their project, that is essentially avoiding the spirit of a computer graphics project using OpenGL which we have been learning all semester.
		- If you're not using shaders anywhere in your project, that's probably a red flag, and you need to come talk to me about what you are planning for your project.
5. Again, **You cannot**  use pygame and pyopengl and do some random project without talking to me first.
6. Again, **You cannot**  use threejs and javascript and do some project without talking to me first.
7. You cannot use other graphics APIS
   	- I'm **Banning all other graphics APIs this term other than the one we use in this class**. Why? Too much copy & paste and occassional cases of plagerism.
   	- Sorry -- I can't evaluate every different APIs with equity.

Make sure you read the above clearly and talk to me or a TA if you have any concern about your project tool stack. C++ and OpenGL like we've been doing for the assignments if fine.

**Anything outside that toolstack (C++, OpenGL, and SDL) needs explicit approval.**

## Dependencies

- You should not introduce dependencies (e.g. libraries such as: glfw, stb_image, etc.) without getting them approved first.
	- I understand many tutorials use glfw, glew, etc. instead of SDL -- but we have been using SDL all semester, so that is my expectation unless you otherwise have a good expectation.
 	- Often times when I see glfw in a project, that tells me you did not understand the purpose of SDL in the first place (which is to merely setup OpenGL context, create a window, and handle input).
    	- Thus -- **glfw** is an example dependency that is **not allowed** unless you otherwise provide a justification for it.

## Assignment strategy

My suggested strategy for this project is to:

* You will have plenty of time to think about the project you want to deliver, but get started early.
* I recommend finding at least one web tutorial, youtube talk, etc. that can serve as a reference for you to fall back on.
* You can get feedback from me ahead of time on nearly any graphics related topic, do so!
* Find your teammates(teams of up to 2) early if you choose to work in a team!
  * Piazza is a good resource, and I will tentatively match folks on a spreadsheet if requested.
  
## How to run your final project

Use a [build.py](./build.py). You can modify [build.py](./build.py) to your needs, but I should be able to type `python3 build.py` and then run the generated executable. I plan to spend 0 time figuring out what dependencies to download. If there is some setup extraneous setup needed to run your project, we should arrange to demonstrate your project during office hours. 

Pretend you are deploying this software to someone who has no idea what your code does and needs to be able to run it. The only assumption you should make is the user has SDL2 setup and perhaps a web browser. Logistically, using the [build.py](./build.py) that we have used in this course makes it possible for myself and the course staff to run and debug your project.

## Video Deliverable

For this project, you need to make a video recording of your project. Here are the specifications:

- Give an 2-3 minute overview of your implementation as a youtube video.
- Explain what your project is (i.e. You should be talking in your video as you show things)
- Show what was achieved (e.g. moving a camera around and talking over) 
- In the video highlight **one** particular technical achievement.
  - e.g. Part of the code or algorithm that does something neat or was difficult to get right.
    - Give a quick 'tutorial' like summary of how you solved the problem.
    - You can talk about other important features in your project, but dedicate some time on one specific item.
    
Videos may publicly be uploaded to the course websites 'hall of fame'--please keep the links up!

### Sample Video Deliverable

Here are some samples from previous years and what I like about it:

- [Terrain](https://youtu.be/cGB8C6IT8eE)
  - I like this sample because:
    - I can hear the speakers voice.
    - The talk is well outlined and concise.
    - The speaker shows off the technical parts of the code well.
    - The speaker shows the actual visuals and uses a high resolution video.
    - It is not a very long video with lots of silence or 'umms'
    - If I were to see this on a resume, I would have a very good idea of what the developer knows.
- (Summer 20) [Breakout](https://www.youtube.com/watch?v=IV4EvMX9Lw8&feature=youtu.be): 
  - I like this sample because:
    - I can again hear the speakers voice very clearly.
    - There are additions beyond what the tutorial provides showing creativity, and pushing the envelope.
    - The presentations highlights very clearly the graphics effects being talked about.
    - The technical portions are broken into small pieces, thus making it possible for someone to learn from. They are teaching as they are presenting their project which is absolutely wonderful.
 - (Fall 20) [Deferred Rendering](https://www.youtube.com/watch?v=S-fF5R1jeBc): 
   - I like this project because this student took a large risk in the implementation.
     - They did a great implementation and conveyed their passion.
     - They conveyed enough passion throughout the entire semester, did something very difficult, that I forgave them for having a very loud fan in their video.
       - They should probably do another recording for their portfolio however.
        - They spent most of the video 'teaching' so I forgave them for going over the recommended 3 minute limit.
- (Fall 20) [3D Sine Wave Visualier](https://www.youtube.com/watch?v=Cu3k_METjzA): 
    - Excellent job teaching in this presentation.
    - May have liked to see the visualization earlier to *sell me* as a viewer, but otherwise speaker did a great and clear presentation.
- (Fall 21) [Surface Simplification](https://www.youtube.com/watch?v=qfl6fkYbPXU): 
	- This project implements an actual SIGGRAPH paper.
	- Well presented, and shows off the actual 3D object going simplification as speaker speaks.
	- Breaks down complicated technical idea (from a paper) to something consumable.


## Deliverables

* You need to commit your code to this repository.
* Your program needs to compile using a build.py script (Unless it is an instructor approved webgl project).
* Put a link to a youtube/Vimeo video in this README.md.
* Put screenshots in the [../README.md](../README.md) along with the youtube/vimeo video.

## More Resources

* N/A

## Other FAQ

- Q: May I upload my final project to a public repository after the semester is over?
	- A: Yes under the following conditions
		- You acknowledge your team members and any third parties fairly for their contributions.
  		- You remove the README.md provided with the assignment (delete it entirely from your github history), as you'll want your own readme anyway.
- Q: May I have a team of 3 or 4--I really want 3 or 4 team members?
	- A: No. I recommend a team size of 1-2. This project can be completed as a 1 person project.
 	- I **strongly** recommend using this project as a portfolio piece and treating it as such if you were to apply for a software or graphics engineering position.
- Q: Can I use any of the lab/support code in my project?
	- A: Yes, anything that I've provided you can use.
 	- Please acknowledge me somewhere in the repo, and if you use this project in your portfolio, please keep that acknowledgement in if there was significant use of code I have provided.
- Q: Can I use late day tokens on the final project?
	- A: No, unfortunately the final project deadline is the same for everyone for both fairness, as well as the more pragmatic grading deadline that looms at the end of the semester.
- Q: Can I have an extension?
	- A: No (see above). The project is released well ahead of time for you to plan.
 	- Do not procrastinate and try to finish the project the weekend before the deadline. 
	
	
### Even more Project Ideas

## Project Ideas

I am going to list a few project ideas that I think are within the scope of this class.

* Procedural generation
  * Terrain rendering [https://www.youtube.com/watch?v=QjTPIbJNO6I]
  * City [https://www.youtube.com/watch?v=-d2-PtK4F6Y]
  * Continuous LOD [https://www.youtube.com/watch?v=DUxAyfuJULc]
* Rendering 
  * Photon maps [https://www.youtube.com/watch?v=GckOkpeJ3BY]
  * Radiosity [https://www.youtube.com/watch?v=8i2M255Zw9I]
* Constructive Solid Geometry
  * [https://www.youtube.com/watch?v=-nWPZgxYW-k]
* Build out or extend ray tracer(Our final homework in some iterations of the course): [https://www.youtube.com/watch?v=ijqaBhyXN98]
  * Depth of field
  * Motion blur
* Modeling
  * Progressive Mesh https://www.youtube.com/watch?v=CcomX9UQO1w
  * Subdivision of surfaces [https://www.youtube.com/watch?v=g1M2y1bZOPw]
  * Or the opposite decimation (mesh simplification) [https://www.youtube.com/watch?v=AvSgHhA9yP4]
  * Deformable mesh [https://www.youtube.com/watch?v=r_rkSi8VZj0]
* Volume rendering
  * Example [https://www.youtube.com/watch?v=nmdlGlJ-2G0]
* Animation
  * Flocking behavior [https://www.youtube.com/watch?v=1svKGmGCdXk]
  * Inverse kinematics [https://www.youtube.com/watch?v=uN_pR9fH-WY]
  * Physics-based simulation (e.g. pinball)
  * Collision detection [https://www.youtube.com/watch?v=ZU5PKXxTv8k]
* Particle Simulation [https://www.youtube.com/watch?v=RMfsdUKEmDo]
* Games
  * Build a little First person perspective(shooter, advenutre, etc.) or other interactive environment
  * Racing game [https://www.youtube.com/watch?v=SVzFWnadOdY]
* Shaders
  * Create a world with many different shaders
    * Cel / Toon shaders [https://www.youtube.com/watch?v=RWTQuvXm2HU]
    * Bump / Normal / Parallax / Relief mapping [Example]
  * Deferred rendering [https://www.youtube.com/watch?v=HIrq4sF5QO8]

