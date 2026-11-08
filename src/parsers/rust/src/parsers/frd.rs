//! Provides the frd parser

use super::Parser;
use crate::error::{ParseError, Result};

#[derive(Debug, Default, Clone)]
pub struct FRDParser {}

impl Parser for FRDParser {
    type Output = String;

    fn parse(&self, input: &str) -> Result<Self::Output> {
        Ok("Success".to_string())
    }
}
