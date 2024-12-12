## Title: 

Animated Textures using GIFs

## YouTube/Dropbox/Drive Link: 

https://youtu.be/wsKJCH_4xos

## Screenshots

Goal: Add support for using animated GIFs in place of PPM images for OBJ model textures.
![chapel-ezgif com-optimize](https://github.com/user-attachments/assets/6e7c0dc5-cd10-4083-b548-f6d4b2bc3b17)

Result (still frame):

![image](https://github.com/user-attachments/assets/5e795b65-f8fb-4525-97f8-6f0905f25f67)

Result (gif):

![fire_chapel](https://github.com/user-attachments/assets/0ed8a3ff-c519-45e8-8d3b-a272e604a449)


Note: Credit goes to Mike Shah for most of the code in the repository. My changes are primarily in the Image.cpp and Image.hpp files.

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
