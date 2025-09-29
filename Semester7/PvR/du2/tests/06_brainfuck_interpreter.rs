//! Run this file with `cargo test --test 06_brainfuck_interpreter`.

// TODO (bonus): Create an interpreter for the [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) language.
// The Brainfuck program will be parsed out of a string and represented as a struct.
//
// Handle both parsing and execution errors using enums representing error conditions,
// see tests for details.
// A parsing error can be either an unknown instruction or an unpaired loop instruction.
// An execution error can be either that the program tries to read input, but there is no more
// input available, or when the program executes more than 10000 instructions (which probably
// signals an infinite loop).
//
// Hint: Put `#[derive(Debug, Eq, PartialEq)]` on top of `ParseError`, `ExecuteError` and `Program`
// (and any other custom types nested inside them) so that asserts in tests work.

#[derive(Debug, Eq, PartialEq)]
enum ParseError {
    UnknownInstruction { location: usize, instruction: char },
    UnmatchedLoop { location: usize },
}
#[derive(Debug, Eq, PartialEq)]
enum ExecuteError {
    NoInputLeft,
    InfiniteLoop,
}
#[derive(Debug, Eq, PartialEq)]
enum Instruction {
    IncrementDP,
    DecrementDP,
    IncrementB,
    DecrementB,
    AcceptByte,
    OutputByte,
    LoopStart { loop_end: usize },
    LoopEnd { loop_start: usize },
}
#[derive(Debug, Eq, PartialEq)]
struct Program {
    instructions: Vec<Instruction>,
}

impl Program {
    fn execute(&self, input: Vec<u8>, memory: Vec<u8>) -> Result<String, ExecuteError> {
        let mut memory = memory;

        let mut ip = 0;
        let mut instructions_executed = 0;
        let mut data_pointer = 0;
        let mut input_pointer = 0;

        let mut output = String::new();

        loop {
            if ip >= self.instructions.len() {
                break;
            }

            if instructions_executed > 1000 {
                return Err(ExecuteError::InfiniteLoop);
            }

            let instruction = &self.instructions[ip];

            match instruction {
                Instruction::IncrementDP => {
                    data_pointer += 1;
                }
                Instruction::DecrementDP => {
                    data_pointer -= 1;
                }
                Instruction::IncrementB => {
                    memory[data_pointer] += 1;
                }
                Instruction::DecrementB => {
                    memory[data_pointer] -= 1;
                }
                Instruction::AcceptByte => {
                    if input_pointer >= input.len() {
                        return Err(ExecuteError::NoInputLeft);
                    }

                    memory[data_pointer] = input[input_pointer];
                    input_pointer += 1;
                }
                Instruction::OutputByte => {
                    output.push(
                        memory[data_pointer] as char, /* 100% safe conversion :) */
                    );
                }
                Instruction::LoopStart { loop_end } => {
                    if memory[data_pointer] == 0 {
                        ip = *loop_end;
                    }
                }
                Instruction::LoopEnd { loop_start } => {
                    if memory[data_pointer] != 0 {
                        ip = *loop_start;
                    }
                }
            }

            ip += 1;
            instructions_executed += 1;
        }

        Ok(output)
    }
}

fn parse_program(program: &str) -> Result<Program, ParseError> {
    let mut stack = Vec::<usize>::new();

    let chars = program.chars();
    let mut instructions = Vec::<Instruction>::with_capacity(chars.clone().count());
    for (i, char) in chars.enumerate() {
        match char {
            '>' => instructions.push(Instruction::IncrementDP),
            '<' => instructions.push(Instruction::DecrementDP),
            '+' => instructions.push(Instruction::IncrementB),
            '-' => instructions.push(Instruction::DecrementB),
            '.' => instructions.push(Instruction::OutputByte),
            ',' => instructions.push(Instruction::AcceptByte),
            '[' => {
                stack.push(i);
                instructions.push(Instruction::LoopStart {
                    loop_end: 0, /* Temporary value, which will be changed at the end */
                });
            }
            ']' => match stack.pop() {
                None => return Err(ParseError::UnmatchedLoop { location: i }),
                Some(start) => {
                    instructions.push(Instruction::LoopEnd { loop_start: start });
                    instructions[start] = Instruction::LoopStart { loop_end: i }; // Now we update to correct end index
                }
            },
            _ => {
                return Err(ParseError::UnknownInstruction {
                    location: i,
                    instruction: char,
                });
            }
        }
    }

    if !stack.is_empty() {
        return Err(ParseError::UnmatchedLoop { location: stack[0] });
    }

    Ok(Program { instructions })
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use crate::{ExecuteError, ParseError, parse_program};

    #[test]
    fn parse_empty() {
        check_output("", "", "");
    }

    #[test]
    fn parse_unknown_instruction() {
        assert!(matches!(
            parse_program(">p"),
            Err(ParseError::UnknownInstruction {
                location: 1,
                instruction: 'p'
            })
        ));
    }

    #[test]
    fn parse_unmatched_loop_start() {
        assert_eq!(
            parse_program(">++[+>][++>"),
            Err(ParseError::UnmatchedLoop { location: 7 })
        );
    }

    #[test]
    fn parse_unmatched_loop_end() {
        assert_eq!(
            parse_program(">++[+>][++>]+]"),
            Err(ParseError::UnmatchedLoop { location: 13 })
        );
    }

    #[test]
    fn missing_input() {
        let program = parse_program(",").unwrap();
        let result = program.execute(vec![], vec![0; 30000]);
        assert_eq!(result, Err(ExecuteError::NoInputLeft));
    }

    #[test]
    fn infinite_loop() {
        let program = parse_program("+[]").unwrap();
        let result = program.execute(vec![], vec![0; 30000]);
        assert_eq!(result, Err(ExecuteError::InfiniteLoop));
    }

    #[test]
    fn copy_input() {
        check_output(",.>,.>,.>,.>,.", "hello", "hello");
    }

    #[test]
    fn output_exclamation_mark() {
        check_output("+++++++++++++++++++++++++++++++++.", "", "!");
    }

    #[test]
    fn three_exclamation_marks() {
        check_output(">+++++++++++++++++++++++++++++++++<+++[>.<-]", "", "!!!");
    }

    #[test]
    fn hello_world() {
        check_output(
            "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.",
            "",
            "Hello World!\n",
        );
    }

    fn check_output(program_text: &str, input: &str, expected_output: &str) {
        let program = parse_program(program_text);
        match program {
            Ok(program) => {
                let result = program
                    .execute(input.to_string().into_bytes(), vec![0; 30000])
                    .expect(&format!("Cannot execute program {program_text}"));
                assert_eq!(result, expected_output);
            }
            Err(error) => {
                panic!("Error occurred while parsing program {program_text}: {error:?}");
            }
        }
    }
}
