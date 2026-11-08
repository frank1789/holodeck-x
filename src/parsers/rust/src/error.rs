//! Provides error descriptions

use std::fmt;

#[derive(Debug)]
pub enum ParseError {
    Empty,
    InvalidLine { line: usize, reason: String },
}

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            ParseError::Empty => write!(f, "input is empty"),
            ParseError::InvalidLine { line, reason } => {
                write!(f, "invalid line {line}: {reason}")
            }
        }
    }
}

impl std::error::Error for ParseError {}

pub type Result<T> = std::result::Result<T, ParseError>;
