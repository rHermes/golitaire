# Golitaire

OpenGL ES 3.0 solitaire in C++20. Very much work in progress.

Attempt to learn OpenGL and graphics programming, by making a very
simple solitaire game.

Libraries used:

- `stb_image.h`: image reading
- `stb_image_writer.h`: debugging
- `fmt`: String formatting
- `spdlog`: logging
- `GLFW3`: window and input managment
- `glad`: opengl loading
- `glm`: Mathematics

## How to build

```
git clone --recursive https://github.com/rhermes/golitaire
cd golitaire
cmake -S . -B build -DCMAKE_BUILD_TYPE:STRING=Release
cmake --build build --config Release
```

Now to run the program, you need to go into the directory and run it:

```
cd build/src/
./golitaire
```

## Things for next time

These are some random thoughts that I think I would have done differently if I had done
this again.

### Cards being tied to a scene renderer

Right now cards are standalone, and are added to a scene renderer to render them. I
think that I would have inverted this paradigm.

If we asked the scene renderer for a card, and each card stored a pointer to the scene
renderer that created it, we could do some nice things. We could do away with shared pointers,
instead having normal pointers.

We could also mark the card order as dirty if a card was changed, because we know when
a card has been changed, and can mark the renderer as dirty. This also would allow us to
do optimizations in the renderer, with updating the geometry of cards, if they are marked
as dirty on next render.

It would of course tie us much stronger to the scene renderer, but I think that is the way
to go in the future.

## Things I learned from this project

### The C++ `mutable` keyword

I hadn't really used this keyword ever before, but it turns out to be a perfect
match for many of the caching operations in the code. Examples are the
`getTransform()` from `Item3D`, which is logically a `const` operation, but
since we lazily update the transform, it couldn't be marked as such. By using
`mutable`, we can get the correct semantics logically, and keep the performance
speedup.

I also used this for caching uniform names in `ShaderProgram`.

The potential downside of this of course, is that you open yourself to foot guns
related to changing these members in functions which they really should be `const`.
I wish there was some sort of keyword that would mark a member as `mutable` only
within that one function.

### Quaternions & Gimbal lock

I didn't really know what quaternions was before starting this, but I now have a
rudimentary understanding.

I also understand what Gimbal lock is now. I understood the problem before, but
not why it was prevented with quaternions. I understand now that the primary
reason they solve the problem, is simply that you move away from storing the
3 rotations in "pitch", "yaw" and "roll", where the order where they are applied
matter, and instead view each rotation as independent. This can cause some headaches,
but it avoids the Gimbal lock problem.

It is important to note that this could be done without quaternions also, but
they have other good properties, like nice interpolation between rotations.

### OpenGL pipeline stalls are a thing

This makes sense really, as I've done some vulkan programing before, and there you
have to be quite specific with which objects goes where, to avoid concurrency issues.

It seems that you could run into this problem with modifying the contents of buffer objects,
if there is an asyncronos draw going on at the same time.

The solution to this seems to be to have two draw buffers for everything, but this
doesn't seem right to me. I'll have to consult on the internet about this.

One solution seems to be to never use `glBufferSubData`, but instead always use
`glBufferData`. This idiom requires more memory, but it ensures that the old data
will be kept around for as long as it's referenced by something else, and then it
will be removed. The new data is completely new. This way there is no backwards
dependency. And no stalls.

As long as this is done with the exact same size, the driver might be able to reuse
the buffer later. It is important to realize that this is just another form of
multibuffering, where you have multiple buffers allocated for each purpose, except
that you get some of the managment aspects for free.

Some drivers don't properly implement this, and so you can get problems on them,
but I'll try it for now and see how it goes.

Another point here, you can also have multiple buffers like suggested earlier,
in a sort of buffer pool. This is essentially the solution outlined above,
but you retain more control over the buffers. This has become the standard
solution for vulkan, so I'm sure I'll have to touch that can of worms later.


Resources:
- https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
- https://gdcvault.com/play/1020791/
- http://www.diva-portal.org/smash/get/diva2:839513/FULLTEXT02
- https://developer.arm.com/documentation/101897/0300/Buffers-and-textures/Buffer-update-for-OpenGL-ES
- https://developer.samsung.com/galaxy-gamedev/resources/articles/opengl.html


### Need to use `public` keyword when deriving
So I didn't know this, but in order for the subclass to be able to be passed
to a function which expects the base class, you need to inherit with the
`public` keyword. This is rather smart, taken from [this answer](https://stackoverflow.com/a/31506864):

> `public` inheritance means "is-a". If you write that `class D` ("Derived") publicly inherits
> from `class B` ("Base"), you are telling C++ compilers (as well as human readers of your code) 
> that every object of type `D` is also an object of type `B`, but *not vice versa*

And

> `private` inheritance means "is-implemented-in-terms-of"

This is a subtle difference, but now I know the difference!


## Attributions

```
Vector Playing Cards 3.2
https://totalnonsense.com/open-source-vector-playing-cards/
Copyright 2011,2021 – Chris Aguilar – conjurenation@gmail.com
Licensed under: LGPL 3.0 - https://www.gnu.org/licenses/lgpl-3.0.html
```
