# CCRMA-256A-HW1
256A first homework submission


## Summary

This is a Additive synthesis synthesizer.
It is intended to be played with your mouse on a single box on the lower part of it showing a complete octave of musical notes (C to C)

The sinusoids involved in the synthesis are related through musical intervals expressed in semitones and
tunning intervals expressed in cents.

The user also has access to each of the partial's gain.

A general asdr manages the envelope of the sum of the sinusoids

Someother functionalities include an octave selector and a Main Level fader

## Usage

It is intended to be played with a mouse or two as shown in the video.
horizontal position of the mouse inside the playable area determines the basefrequency and therefore all of the frequencies by all of the sinusoids
Vertical position of the mouse determine the volume or "velocity" of each note. However this value may change during the course of a note.

## Code Description

The whole project is structured on the MainProcessor class and the MainEditor class.
The MainProcessor will be the holder for all the audio related stuff such as the oscillators, the envelope and obviously the  process block itself.

The MainEditor will be responsible for all the gui and the interaction with the user through sliders and mouseListeners
therefore it will inherit from sliderListener class.

There is a special relation between the processor and the editor.
The editor is created by the processor which during its initialization passes a pointer to the editor object of itself;
therefore the editor has access to all of the processor's public methods and fiels and the processor has access to all of
the editors public methods and fields and is actually the holder for the editor taking care of its lifetime.

Take into account that this requires a circular inclussion of the processor's header on the  editor's header and also of the editor's header in the processor's header. To avoid this a dummy class is created on the processor's header for it to be able to create the pointer that will store, during initialization, the address of the editor. This is the main reason for splitting the project into .cpp and .h files in the editor aswell as in the processor part of the program

## Example of Use

see this youTube video to check how the synthesizer works and sounds ;)

https://youtu.be/bd0QLtfIBxQ


## History

Single Commit repo

## Contributing

Created by Juan Sierra
Contributions and greatly appreciated advices by Megan and Rahul

## License

MIT License

Copyright (c) 2016 - Juan Sierra, JuanSaudio

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
