USE [uu278]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [tbluAccount]
(
	--[ID] [int] IDENTITY(1,1) NOT NULL,											
	[UserID] [int] IDENTITY(1,1) NOT NULL,											--�û�ID
	[UserName] [varchar](32) COLLATE Chinese_PRC_CI_AS NOT NULL,					--�û���
	[PassWord] [varchar](32) COLLATE Chinese_PRC_CI_AS NOT NULL,					--�û�����
	--[GLevel] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GLevel] DEFAULT(1),		--����ȼ���Ĭ��1��
	[CreateTime] [datetime] NULL CONSTRAINT [DF_tbluAccount_CreateTime] DEFAULT(getdate()),--���崴��ʱ��Ĭ�ϵ�ǰʱ��
	--[UpdateTime] [datetime] NULL,													--������Ϣ����ʱ��
	--[bRefuseJoin] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GRefuse] DEFAULT(0),	--�Ƿ�ܾ���Ҽ������
	CONSTRAINT [PK_tbluAccount] PRIMARY KEY CLUSTERED									--��������
	(
		[UserID]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--�����������,�ӿ��ȡ�ٶ�
) ON [PRIMARY]

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
---------------------------------------------------------------------------�����洢����---------------------------------------

/****** Object:  StoredProcedure [dbo].[GSP_GG_AccountInfo_GetInfo]    Script Date: 2015/8/28 9:53:49 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[GSP_GG_AccountInfo_GetInfo]
	@iUserID int 				--���Ψһ��ʶ
AS
	SET NOCOUNT ON
	
	--���������Ƿ��Ѿ�����
	IF NOT EXISTS(SELECT UserID FROM tbluAccount WHERE UserID = @iUserID)
	BEGIN 
		RETURN -1
	END

	IF @iUserID < 0 
	BEGIN
		RETURN -2
	END
	

	SELECT UserID,UserName,PassWord,CreateTime FROM tbluAccount WHERE UserID = @iUserID

	return 0
SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON


/****** Object:  StoredProcedure [dbo].[GSP_GG_AccountInfo_GetInfoByUserName]    Script Date: 2015/8/28 9:51:39 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[GSP_GG_AccountInfo_GetInfoByUserName]
	@strUserName varchar(25) 				--���Ψһ��ʶ
AS
	SET NOCOUNT ON
	
	--���������Ƿ��Ѿ�����
	IF NOT EXISTS(SELECT UserID FROM tbluAccount WHERE UserName = @strUserName)
	BEGIN 
		RETURN -1
	END

	SELECT UserID,UserName,PassWord,CreateTime FROM tbluAccount WHERE UserName = @strUserName

	return 0
SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON


SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[GSP_GG_Add_Account]
	@iUserID int OUTPUT,				--���Ψһ��ʶ
	@szUserName varchar(32),
	@szPassWord varchar(32)
AS
	SET NOCOUNT ON
	BEGIN TRAN
	
	IF EXISTS(SELECT @iUserID FROM tbluAccount WHERE UserID = @iUserID)
	BEGIN 
		ROLLBACK TRAN
		RETURN -1
	END

	IF @iUserID < 0 
	BEGIN
		ROLLBACK TRAN
		RETURN -2
	END
	
	SET @iUserID = @@IDENTITY

	INSERT INTO tbluAccount(UserName,PassWord,CreateTime) VALUES (@szUserName,@szPassWord,GetDate());
	

	COMMIT TRAN

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON