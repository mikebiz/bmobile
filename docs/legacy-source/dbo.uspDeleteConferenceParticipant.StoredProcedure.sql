/****** Object:  StoredProcedure [dbo].[uspDeleteConferenceParticipant]    Script Date: 08/27/2008 14:36:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspDeleteConferenceParticipant] 
(
	@ConferenceID uniqueidentifier
	, @ParticipantID uniqueidentifier
) 
AS 

BEGIN

	DECLARE	@_error int
	DECLARE @_errorString char( 255 )
	DECLARE @PptParticipantID uniqueidentifier

BEGIN DISTRIBUTED TRANSACTION

	UPDATE utConferenceParticipant
	SET isActive = 0
	WHERE ConferenceID = @ConferenceId
		AND ParticipantID = @ParticipantID

	IF 0 = @@ROWCOUNT
	BEGIN
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ParticipantID ) )
		RAISERROR( 
			'Participant %d  does not exist for Conference:'
			, 16
			, 1
			, 0
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1
	END

	SELECT @PptParticipantID = CP.ID
	FROM utConferenceaParticipant AS CP
	WHERE ConferenceID = @ConferenceId
		AND ParticipantID = @ParticipantID

	EXECUTE @_error = uspPublishingPointDestinationRemove
					@PptParticipantId

	IF @_error <> 0
	BEGIN
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @PptParticipantId ) )
		RAISERROR( 
			'Participant destination %d  cannot be removed:'
			, 16
			, 1
			, 0
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1
	END


	EXECUTE @_error = uspPublishingPointSourceRemoveAudio
					@PptParticipantId

	IF @_error <> 0
	BEGIN
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @PptParticipantId ) )
		RAISERROR( 
			'Participant source audio %d  cannot be removed:'
			, 16
			, 1
			, 0
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1
	END

	EXECUTE @_error = uspPublishingPointSourceRemoveVideo
					@PptParticipantId

	IF @_error <> 0
	BEGIN
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @PptParticipantId ) )
		RAISERROR( 
			'Participant source video %d  cannot be removed:'
			, 16
			, 1
			, 0
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1
	END

COMMIT TRANSACTION

END
GO
