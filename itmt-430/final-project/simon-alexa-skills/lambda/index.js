/**
 * 
 * This program demonstrates Simon game with Alexa skills.
 * Class is Smart Technologies at IIT
 * 
 * Myungji Jeong
 * 
 **/

const Alexa = require( 'ask-sdk-core' );
const Util = require( './util' );

var score = 0  

var colors=["red", "blue", "green", "yellow"]

// ramdom machine generate pattern store in memory
var gamePattern = new Array(4); 
// ramdom user clikc pattern  in memory store
var userPattern = new Array(4);

const PlayPrompt = "What would you like to play?"


// Selects a random confirmation prefix word.
function randomConfirmation()
{
    var Response = "Okay, ";
    var option = Math.floor( Math.random() * 4 );

    if( option === 0 ) { Response = "Okay, "; }
    else if( option === 1 ) { Response = "Sure, "; }
    else if( option === 2 ) { Response = "Allright, "; }
    else { Response = "You got it, "; }
    
    return Response;
}

// Selects a random failure prefix word.
function randomFail()
{
    var Response = "Fail!! ";
    var option = Math.floor( Math.random() * 4 );

    if( option === 0 ) { Response = "Fail!! "; }
    else if( option === 1 ) { Response = "Too bad, "; }
    else if( option === 2 ) { Response = "Sorry, "; }
    else { Response = "Oh no, "; }
    
    return Response;
}


// Selects a random success prefix word.
function randomSuccess()
{
    var Response = "Congratulations! ";
    var option = Math.floor( Math.random() * 3 );

    if( option === 0 ) { Response = "Congratulations! "; }
    else if( option === 1 ) { Response = "Great job! "; }
    else { Response = "Good job! "; }
    
    return Response;
}

const LaunchRequestHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    handle(handlerInput)
    {
        return handlerInput.responseBuilder
            .speak( PlayPrompt )
            .reprompt( PlayPrompt )
            .getResponse();
    }
};

//
// Handler for when the use asks what games can be played.
//
const GameQueryIntentHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'GameQueryIntent';
    },
    handle(handlerInput)
    {
        return handlerInput.responseBuilder
            .speak("You can play Simon game.")
            .reprompt(PlayPrompt)
            .getResponse();
    }
}

//
// Handler for when the use asks for instructions on how to play a game.
//
const InstructionsIntentHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'InstructionsIntent';
    },
    handle(handlerInput)
    {
        var speechOutput = "";
        
        speechOutput = `After I read a list of colors, say the corresponding colors in the same order. `
        speechOutput += "I will read randomly 4 colors to the sequence. "
        speechOutput += "If you want to play game, say START!"
        
        return handlerInput.responseBuilder
            .speak(speechOutput)
            .reprompt(speechOutput)
            .getResponse();
    }
}

//
// Handler for when the user asks to play Simon.
//
const SimonIntentHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'SimonIntent';
    },
    handle(handlerInput)
    {
        return handlerInput.responseBuilder
            .speak(randomConfirmation() + "let's play Simon.")
            .getResponse();
    }
};

//
// Handler for when Simon starts
//
const PatternIntentHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'PatternIntent';
    },
    handle(handlerInput)
    {
        var patterns = "";
        for(var i=0;i<colors.length;i++){
            var random = Math.floor(Math.random() * Math.floor(4));
            
            gamePattern[i] = colors[random]
            patterns += " " + gamePattern[i];
        }
        return handlerInput.responseBuilder
            .speak("Your pattern is" + patterns + ". Now, your turn.")
            .reprompt("Your pattern is " + patterns)
            .getResponse();
    }
};

//
// Handler for when Simon starts
//
const MatchPatternIntentHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'MatchPatternIntent';
    },
    handle(handlerInput)
    {
        var correctFlag = 0;
        var patterns = "";
        var statusString = "";
        const slots = handlerInput.requestEnvelope.request.intent.slots;
        
        userPattern[0] = slots["colorOne"].value;
        userPattern[1] = slots["colorTwo"].value;
        userPattern[2] = slots["colorThree"].value;
        userPattern[3] = slots["colorFour"].value;
        
        for(var i=0;i<4;i++){
            if(correctFlag === 0){
                if(userPattern[i] === gamePattern[i]){ correctFlag = 0; }
                else{
                    correctFlag = 1;
                    var totalScore = score;
                    score = 0;
                    statusString = randomFail() + "Your total score is " + totalScore + ". ";
                    statusString += "say START to start again! or STOP.";
                }
            }
            patterns += userPattern[i] + " ";
        }
        if(correctFlag === 0){
            score = score + 1;
            statusString = randomSuccess() + " Now score is " + score + ". ";
            statusString += "Say GO to continue! or STOP.";
        }
        
        
        return handlerInput.responseBuilder
            .speak(statusString)
            .reprompt(statusString)
            .getResponse();
    }
};


const HelpIntentHandler = {
    canHandle( handlerInput )
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.HelpIntent';
    },
    handle( handlerInput )
    {
        const speakOutput = 'You can play Simon.';

        return handlerInput.responseBuilder
            .speak( speakOutput )
            .getResponse();
    }
};

const CancelAndStopIntentHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && (Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.CancelIntent'
                || Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.StopIntent');
    },
    handle( handlerInput )
    {
        const speakOutput = 'Thanks for playing!';
        return handlerInput.responseBuilder
            .speak( speakOutput )
            .getResponse();
    }
};

const SessionEndedRequestHandler = {
    canHandle(handlerInput)
    {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'SessionEndedRequest';
    },
    handle(handlerInput)
    {
        return handlerInput.responseBuilder
            .speak( "Thanks for playing!" )
            .getResponse();
    }
};


// Generic error handling to capture any syntax or routing errors. If you receive an error
// stating the request handler chain is not found, you have not implemented a handler for
// the intent being invoked or included it in the skill builder below.
const ErrorHandler = {
    canHandle()
    {
        return true;
    },
    handle( handlerInput, error )
    {
        console.log( `!!!! Error handled: ${error.stack}` );
        const speakOutput = `Sorry, something went wrong. Please try again.`;

        return handlerInput.responseBuilder
            .speak( speakOutput )
            .reprompt( speakOutput )
            .getResponse();
    }
};

// The request interceptor is used for request handling testing and debugging.
// It will simply log the request in raw json format before any processing is performed.
const RequestInterceptor = {
    process(handlerInput)
    {
        let { attributesManager, requestEnvelope } = handlerInput;
        let sessionAttributes = attributesManager.getSessionAttributes();

        // Log the request for debug purposes.
        console.log(`=====Request==${JSON.stringify(requestEnvelope)}`);
        console.log(`=========SessionAttributes==${JSON.stringify(sessionAttributes, null, 2)}`);
    }
};



// The SkillBuilder acts as the entry point for your skill, routing all request and response
// payloads to the handlers above. Make sure any new handlers or interceptors you've
// defined are included below. The order matters - they're processed top to bottom.
exports.handler = Alexa.SkillBuilders.custom()
    .addRequestHandlers(
        LaunchRequestHandler,
        GameQueryIntentHandler,
        InstructionsIntentHandler,
        SimonIntentHandler,
        PatternIntentHandler,
        MatchPatternIntentHandler,
        HelpIntentHandler,
        CancelAndStopIntentHandler,
        SessionEndedRequestHandler
        )
    .addRequestInterceptors( RequestInterceptor )
    .addErrorHandlers(
        ErrorHandler,
    )
    .lambda();