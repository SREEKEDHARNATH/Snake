# Snake
The classical snake game

So welcome i will be guiding you in here.
make sure assets folder is same in current directory and install sdl_mixer.
So warning you may not like this code as i will admit its structure,methods,code density is bad.
And readme is bad first time, If you thought of any suspicion in commits as progression got low first time gui so taken a lot of time and started sdl 1 and a half weeks before this so pretty bad.
Credits at top mainly for sound's and wallpapers.
No ai i can assure you that.
All assets except then ones in assets/themes are hand drawn.
So i hope you will have the patience.

There are 4 main files first api(both headers and cpp) it consists the code for all basics like a image it handles pointers creation drawing, then we have font creates a font, then text a mix of these instead of a file it asks for text so it makes it own image then we have sound effect and music both are related to sound the difference is in music class you have more control in sound effect class you can only play and set volume this was intentional as no stopping of sound effect meant for sounds < 5seconds.

Now we have the listeners(both headers and cpp) it handles mouse and keyboard inputs, for registering you give the event obtained from "SDL_PollEvent" and a switch statement in main loop. Have isPressed and isDown the logic is we use two bool arrays one for current frame and one for now what we check is if current arrays key is true and others false thats counted as isPressd.

Then we have utils(both headers and cpp) if you check the code you can see its not ustils but main game and utilities combined i know but i thought its ok. In snake class we have arrays of std pair called pos i would use SDL_Rect but width and height is unnecessary as they are same so for physics we use the positions and applewas tough as i couldnt figure out how to get random positions i resorted to std::set put all x of bodies and heads in "allx" variable which is a set same for y then a for loop to loop through it and if same use random and set i to 0. Growing is easy just take tails old position if growing put it at back after body updated. 

Now we have slider the base putting it was easy but the logic had taken my time alot main reason was in sdl orgin is at top left i was thinking of bottom left.

At last checkbox its a simple variable controlling it 1 issue i faced is on shared ptr i used reset but some problem so i used api::image's pointer it worked.

Now window class its init return std pair of bool and const char*, the bool representing it worked and const char* the message to std::cout it inits all and run function checks the bool i mentioned earlier if false(true=worked and vice versa) quit fuction quit all but not called by run because if they were the fonts auto destructor(aka TTF_CloseFont needs the font library to be not closed).It makes all images sounds text checkboxes sliders playbutton etc, then it tries to read from a file called save.conf first line is usic volume second sound third muted fourth show solid colour, this has a lot of themes and solid colour as wallpapers a checkbox for to switch to solid colur and reverse, sliders, mute icon too. A tutorial is also there you can access it by pressing t.

Now if you want read all the code so you can understand even more if any mistakes in this readme i am sorry.

This is it thanks.



Credits:-
Music by <a href="https://pixabay.com/users/soundreality-31074404/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=537925">Jurij</a> from <a href="https://pixabay.com//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=537925">Pixabay</a>

Sound Effect(Apple eat) by <a href="https://pixabay.com/users/freesound_community-46691455/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=108204">freesound_community</a> from <a href="https://pixabay.com/sound-effects//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=108204">Pixabay</a>

Sound Effect(Game Over) by <a href="https://pixabay.com/users/tuomas_data-40753689/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=199830">Tuomas_Data</a> from <a href="https://pixabay.com//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=199830">Pixabay</a>

Sound Effect(Play) by <a href="https://pixabay.com/users/universfield-28281460/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=326133">Official Profile</a> from <a href="https://pixabay.com//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=326133">Pixabay</a>

Theme 1 in <a href="https://in.pinterest.com/pin/873698396450976914/">pintrest</a>

Theme 2 in pintrest from <a href="https://in.pinterest.com/pin/1126322188107584724/">jin</a>

Theme 3 in <a href="https://in.pinterest.com/pin/799811215099707807/">Pintrest</a>

Theme 4 in <a href="https://in.pinterest.com/pin/949344796471847133/">Pintrest</a>

Theme 5 in pintrest from <a href="https://in.pinterest.com/pin/425238389788354076/">maycat</a>

Theme 6 from <a href="https://gallery.yopriceville.com/Backgrounds/Summer_Lawn_with_Daisies_Background">yopriceville</a>

Theme 7 from <a href="https://pngtree.com/freebackground/blurred-park-with-bokeh-light-nature-background_15876102.html?utm_source=Pinterest&utm_medium=organic">pngtree</a>

Theme 8 <a href="http://www.flickr.com/photos/digital_trance/">digital_trance on flickr</a>

Theme 9 in pintrest from <a href="https://in.pinterest.com/pin/800092690095745925/">isthiscloe</a>

Theme 10 in pintrest from <a href="https://in.pinterest.com/pin/1060105199792685738/">Ubgpo</a>
