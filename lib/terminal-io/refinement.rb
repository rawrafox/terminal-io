module TerminalIO
  refine IO do
    def drain
      Termios.drain(self)
    end

    def flow(action)
      Termios.flow(self, action)
    end

    def flow_suspend
      self.flow(Termios::TCOOFF)
    end

    def flow_resume
      self.flow(Termios::TCOON)
    end

    def send_start
      self.flow(Termios::TCION)
    end

    def send_stop
      self.flow(Termios::TCIOFF)
    end

    def send_break(duration: 0)
      Termios.sendbreak(duration)
    end

    def flush_queue(input: true, output: false)
      queue = if input && output
        Termios::TCIOFLUSH
      elsif input
        Termios::TCIFLUSH
      elsif output
        Termios::TCOFLUSH
      else
        return
      end

      Termios.flush(self, queue)
    end

    def getattr
      Termios.getattr(self)
    end

    def setattr(value, action: :now)
      case action
      when :now then Termios::TCSANOW
      when :drain then Termios::TCSADRAIN
      when :flush then Termios::TCSAFLUSH
      else
        action
      end

      Termios.setattr(self, action, value)
    end

    def termios(action: :now, &block)
      self.setattr(block.call(self.getattr), action: action)
      self
    end
  end
end
