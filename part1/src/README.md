## Notes 

- For this assignment, I did not use the starter code and I did not get the
  brick wall to work but I did get the normal mapping to work on loaded objects
  using old code 
- Comparison of without normal mapping (left) and with normal mapping (right) is shown below
![Comparison](../media/image.png)
- Because the effect of the normal mapping was somewhat limited, I made the
  normal map texture more pronounced by multiplying the values and clamping them
  in the fragment shader. The result is a bumpier surface:
![Additional](../media/image-1.png)