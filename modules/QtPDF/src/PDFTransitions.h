/**
 * Copyright (C) 2013  Stefan Löffler
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
#ifndef PDFTransitions_H
#define PDFTransitions_H

#include <QImage>
#include <QTime>

namespace QtPDF {

namespace Transition {

class AbstractTransition
{
public:
  enum Type { Type_Split, Type_Blinds, Type_Box, Type_Wipe, Type_Dissolve,
              Type_Glitter, Type_Replace, Type_Fly, Type_Push, Type_Cover,
              Type_Uncover, Type_Fade };
  enum Motion { Motion_Inward, Motion_Outward };

  AbstractTransition() = default;
  virtual ~AbstractTransition() = default;

  bool isRunning() const { return (_started && !_finished); }
  bool isFinished() const { return _finished; }
  double duration() const { return _duration; }
  int direction() const { return _direction; }
  Motion motion() const { return _motion; }
  void setDuration(const double duration) { _duration = duration; }
  // for valid values, see pdf specs (use -1 for None)
  void setDirection(const int direction) { _direction = direction; }
  void setMotion(const Motion motion) { _motion = motion; }
  
  virtual void start(const QImage & imgStart, const QImage & imgEnd);
  virtual void reset() { _started = _finished = false; }
  virtual QImage getImage() = 0;
  
  static AbstractTransition * newTransition(const Type type);
  
protected:
  double getFracTime();
  virtual void setImages(const QImage & imgStart, const QImage & imgEnd);
  
  double _duration{1};
  int _direction{0};
  Motion _motion{Motion_Inward};
  bool _started{false};
  bool _finished{false};
  QTime _timer;
  QImage _imgStart;
  QImage _imgEnd;
  // TODO: /SS and /B properties
};

class AbstractInPlaceTransition : public AbstractTransition
{
public:
  AbstractInPlaceTransition() = default;
  virtual ~AbstractInPlaceTransition() override = default;
  
  virtual void start(const QImage & imgStart, const QImage & imgEnd) override;
  virtual QImage getImage() override;
protected:
  virtual void initMask() = 0;
  
  QImage _mask;
  double _spread{0.05};
};

class Replace : public AbstractTransition
{
public:
  Replace() = default;
  virtual QImage getImage() override;
};

class Split : public AbstractInPlaceTransition
{
public:
  Split() { _spread = 0; }
protected:
  virtual void initMask() override;
};

class Blinds : public AbstractInPlaceTransition
{
public:
  Blinds() = default;
protected:
  virtual void initMask() override;
  unsigned int _numBlinds{6};
};

class Box : public AbstractInPlaceTransition
{
public:
  Box() { _spread = 0; }
protected:
  virtual void initMask() override;
};

class Wipe : public AbstractInPlaceTransition
{
public:
  Wipe() { _spread = 0; }
protected:
  virtual void initMask() override;
};

class Dissolve : public AbstractInPlaceTransition
{
public:
  Dissolve() = default;
protected:
  virtual void initMask() override;
};

class Glitter : public AbstractInPlaceTransition
{
public:
  Glitter() { _spread = .1; }
protected:
  virtual void initMask() override;
};

class Fly : public AbstractTransition
{
public:
  Fly() = default;
  virtual void start(const QImage & imgStart, const QImage & imgEnd) override;
  virtual QImage getImage() override;
protected:
  QImage _mask;
};

class Push : public AbstractTransition
{
public:
  Push() = default;
  virtual QImage getImage() override;
};

class Cover : public AbstractTransition
{
public:
  Cover() = default;
  virtual QImage getImage() override;
};

class Uncover : public AbstractTransition
{
public:
  Uncover() = default;
  virtual QImage getImage() override;
};

class Fade : public AbstractTransition
{
public:
  Fade() = default;
  virtual QImage getImage() override;
};

} // namespace(Transition)

} // namespace QtPDF

#endif // End header guard
// vim: set sw=2 ts=2 et

