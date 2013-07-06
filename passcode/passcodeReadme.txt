After reading this post on the MakerCamp Google+ community ( https://plus.google.com/118236842579702510118/posts/Vpm9stdhQo2 ),
I got to thinking this could be a good little test for me as I'm learning Arduino. As such, what's in here may not be ideal
for the platform, but it's fairly sane as far as the general idea goes.


NOTES
----------
I've added a servo since taking the breadboard picture. You don't need to add it, but if you want to attach it to power and ground and it's signal to D4. Depending upon
your servo you may need to adjust the angle values (do so by modifying the consts). If you don't have one make sure nothing is plugged into D4 or disable that part of the code.

The debouncing I put in for button presses may not be right for your buttons, adjust those consts as necessary.


OVERVIEW
----------
The approach I've taken here is applying the Knuth-Morris-Pratt algorithm ( Details here for the curious and geeky http://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm )
to this situation. 

Here's a little thinking about why, and an alternative approach. As the question posed was for a chest that looked like a NES controller, that means you've got a limited number
of buttons, and no predefined enter button. If you want to use one as an enter key, you could just build up a string and compare and clear it each time enter is pressed. This would
work fine, but has the drawbacks that you're using a whole button just for enter, if anything gets pressed in between you'll need to clear what's already entered by pressing enter 
yourself, and by pressing too much without pressing enter you'd overflow eventually.

You could go for a fixed length, and just check every X characters instead, but then you'd need to provide feedback on how many characters entered, and to press the number left
to clear in between when buttons get accidentally pushed. This fixes the overflow and waste of key issue, but presents that new issue.

By pretending that all the buttons every pressed are part of an infinite input sequence and that our buttons are a limited set of letters, we pretty much have exactly what the
Knuth-Morris-Pratt algorithm is optimized for. The memory requirement is fixed in relation to the length of the passcode (though as we have no new, I just made a maximum length)
and the number of potential characters.

The code in here could no doubt use some tweaking (for instance it's somewhat wasteful if your passcode is never changing), but it seems to get the job done. The main downside
of this approach is that because there is no enter key random button pressed could wind up unlocking it, but that's true even with an enter key, just that the length need not be
known in this case to press enter at the right time (instead of having to try all lengths).

Anyway, hope this is helpful to someone out there. Happy hacking.

If you have any questions, or would prefer I added some how to adapt this for your project pieces in here, feel free to email me - skyphyr@gmail.com