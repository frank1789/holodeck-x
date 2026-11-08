//! Provides the APDL parser

use super::Parser;
use crate::error::{ParseError, Result};

#[derive(Debug, Default, Clone)]
pub struct APDLParser {}

impl APDLParser {
    /// Constructs a `APDLParser`
    ///
    #[must_use]
    pub fn new() -> APDLParser {
        Self {}
    }
}

impl Parser for APDLParser {
    type Output = String;

    fn parse(&self, input: &str) -> Result<Self::Output> {
        Ok("Success".to_string())
    }
}
