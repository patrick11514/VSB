use std::fmt::Write;

#[derive(Copy, Clone)]
enum MemoryCell {
    Undefined,
    Defined(u8),
}

#[derive(Debug)]
enum ReadExprError {
    Undefined,
    OutOfBounds { index: usize },
}

#[derive(Debug)]
enum WriteExprError {
    OutOfBounds { index: usize },
}

struct Cpu {
    registers: [MemoryCell; 16],
    memory: [MemoryCell; 1024],
}

impl Cpu {
    fn undefined() -> Self {
        Cpu {
            registers: Vec<MemoryCell>,
            memory: Vec<MemoryCell>,
        }
    }

    fn zeroed() -> Self {
        Cpu {
            registers: [MemoryCell::Defined(0); 16],
            memory: [MemoryCell::Defined(0); 1024],
        }
    }

    fn read(&self, expr: ReadExpr) -> Result<u8, ReadExprError> {
        let (slice, address) = match expr {
            ReadExpr::Constant(value) => return Ok(value),
            ReadExpr::Addr(address) => match address {
                AddrExpr::Memory(addr) => (self.memory.as_slice(), addr as usize),
                AddrExpr::Register(addr) => (self.registers.as_slice(), addr as usize),
            },
        };

        match slice.get(address) {
            Some(cell) => match cell {
                MemoryCell::Undefined => Err(ExprError::Undefined),
                MemoryCell::Defined(value) => Ok(*value),
            },
            None => Err(ExprError::OutOfBounds { index: address }),
        }
    }

    fn write(&mut self, expr: WriteExpr, value: u8) -> Result<(), WriteExpr> {
        let (slice, address) = match expr {
            WriteExpr::Addr(address) => match address {
                AddrExpr::Memory(addr) => (self.memory.as_mut(), addr as usize),
                AddrExpr::Register(addr) => (self.registers.as_mut(), addr as usize),
            },
        };

        match slice.get_mut(address) {
            Some(cell) => match cell {
                MemoryCell::Undefined => Err(ExprError::Undefined),
                MemoryCell::Defined(value) => Ok(*value),
            },
            None => Err(ExprError::OutOfBounds { index: address }),
        }
    }
}

enum AddrExpr {
    Register(u8),
    Memory(u32),
}
enum ReadExpr {
    Constant(u8),
    Addr(AddrExpr),
}

enum WriteExpr {
    Addr(AddrExpr),
}

enum Instruction {
    Set { dest: WriteExpr, src: ReadExpr },
    Print(ReadExpr),
}

#[derive(Debug)]
enum ExecuteError {
    BadExpression(ReadExprError),
}

impl From<ExprError> for ExecuteError {
    fn from(value: ExprError) -> Self {
        Self::BadExpression(value)
    }
}

fn execute_instruction(cpu: &mut Cpu, inst: Instruction) -> Result<(), ExecuteError> {
    match inst {
        Instruction::Print(expr) => {
            /*let value = match cpu.evaluate_expr(expr) {
                Ok(value) => value,
                Err(error) => return Err(error.into()),
            }; stejný kód => */
            let value = cpu.read(expr)?;

            println!("{value}");
        }
    }

    Ok(())
}

fn main() {
    let mut cpu = Cpu::undefined();
    let inst = Instruction::Print(ReadExpr::Constant(5));
    execute_instruction(&mut cpu, inst).unwrap();
}
