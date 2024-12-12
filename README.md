*TODO*: Please edit the following information in your final project

## Title: 

Animated Textures using GIFs

## YouTube/Dropbox/Drive Link: 

**Edit this line -- Put your link to your video(Preferably on YouTube) here (see deliverables)**

## Screenshots

Goal: Allow animated textures like the one below to be used in place of PPM images in the rendering pipeline we used for assignments.
![chapel-ezgif com-optimize](https://github.com/user-attachments/assets/6e7c0dc5-cd10-4083-b548-f6d4b2bc3b17)

Result: Unfortunately I was unable to complete this project, and it fails to correctly load certain GIFs (possibly due to size, or file format details I missed). However in general the GIF parsing functionality seems to work, and I was able to succeed in loading a small sample gif and loading it as a texture.

Failed Example (loaded texture was the above gif):
![image](https://github.com/user-attachments/assets/a3bc8d66-bf24-4411-9ef1-0627407f0feb)

Working Example using a small sample gif:
![image](https://github.com/user-attachments/assets/e834f0d7-dc06-4c0e-a0e1-925d5a74d586)

* Name and partners name(At most teams of 2 folks total)
  * Team member 1: Joseph Hirsch
* How many hours did it take you to complete this final project? 25
* Did you collaborate or share ideas with any other students/TAs/Professors?
* Did you use any external resources? 
  * Note it is fair to utilize some tutorials--cite them here. You **must** add your own personal touch however!
  * GIF file format breakdown: https://giflib.sourceforge.net/whatsinagif/bits_and_bytes.html
  * Official GIF spec (for the lastest format version): https://www.w3.org/Graphics/GIF/spec-gif89a.txt
  * (Optional) What was the most interesting part of the Final Assignment? How could the instructor improve the final project?
  * Building the state machine for loading the GIF. It was complicated and difficult, and slightly aggravating, but a great learning experience especially working with a binary file and the related operations (such as dealing with compressed codes for the color indices which are smaller than a byte). I think overall I am happy I chose to do this project as a result of everything I learned.

### Rubric

**Make sure you read the banned project ideas/APIs in the Readme**

<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>(5%) Project Check-in</td>
     <td align="left">Did you check-in with the professor or TA during office hours regarding your project at least 2 weeks before the deadline?</td>
    </tr>
  </tbody>
</table>


<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>(25%) Project Completion</td>
     <td align="left"><ul><li>(10%) Does the project compile and run.</li><li>Is it polished without any bugs (No weird visual artifacts).</li><li>(15%)Did you make a video (ideally 2-3 minutes in length following guidelines given in writeup: introduce your project, show your project, show something technical in code that you can explain.)?</li><li>(5%)Did you add a screenshot of your project to the repository?</li></ul></td>
    </tr>
    <tr>
      <td>(40%) Technical</td>
      <td align="left"><ul><li>Was the implementation of the project challenging?</li><li>Even if you followed a tutoral, it should not be trivial, and have some personal touch to it.</li><li>Did you have to organize/process a sufficient amount of data?</li><li>Was it clear you consulted some outside resources that go above and beyond the scope of this class</li></ul></td>
    </tr>
    <tr>
      <td>(30%) Creativity</td>
      <td align="left"><ul><li>How visually appealing is the scene?<ul><li>Note: There should be some 'wow' factor--instructors discretion is used here relative to previous projects.</li></ul></li><li>How original is the project<ul><li>Again, did you enhance a tutorial and do something unique or just go by the book?</li></ul></li></ul></td>
    </tr>
  </tbody>
</table>
